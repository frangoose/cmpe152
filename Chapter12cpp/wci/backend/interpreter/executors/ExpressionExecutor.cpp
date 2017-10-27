/**
 * <h1>AssignmentExecutor</h1>
 *
 * <p>Execute an assignment statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include <set>
#include "ExpressionExecutor.h"
#include "StatementExecutor.h"
#include "CallExecutor.h"
#include "../Cell.h"
#include "../RuntimeError.h"
#include "../../../DataValue.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/TypeSpec.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../backend/BackendFactory.h"
#include "../../../backend/interpreter/memoryimpl/MemoryMapImpl.h"
#include "../../../message/Message.h"
#include "../../../backend/interpreter/memoryimpl/CellImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;;
using namespace wci::backend;
using namespace wci::backend::interpreter::memoryimpl;
using namespace wci::message;

set<ICodeNodeTypeImpl> ExpressionExecutor::ARITH_OPS =
{
    NT_ADD, NT_SUBTRACT, NT_MULTIPLY,
    NT_FLOAT_DIVIDE, NT_INTEGER_DIVIDE, NT_MOD,
};

ExpressionExecutor::ExpressionExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

CellValue *ExpressionExecutor::execute(ICodeNode *node)
{
    ICodeNodeTypeImpl node_type = (ICodeNodeTypeImpl) node->get_type();
    CellValue *result_cell_value;

    switch (node_type)
    {
        case NT_VARIABLE:
        {
            // Return the variable's value.
            result_cell_value = execute_value(node);
            break;
        }

        case NT_INTEGER_CONSTANT:
        {
            TypeSpec *typespec = node->get_typespec();
            NodeValue *node_value = node->get_attribute((ICodeKey) VALUE);
            DataValue *value = node_value->value;

            // If boolean, return true or false.
            // Else return the integer value.
            result_cell_value = (typespec == Predefined::boolean_type)
                                    ? new CellValue(value->i == 1)
                                    : new CellValue(value->i);
            break;
        }

        case NT_REAL_CONSTANT:
        {
            // Return the data value.
            NodeValue *node_value = node->get_attribute((ICodeKey) VALUE);
            result_cell_value = new CellValue(node_value->value->f);
            break;
        }

        case NT_STRING_CONSTANT:
        {
            NodeValue *node_value = node->get_attribute((ICodeKey) VALUE);
            DataValue *value = node_value->value;

            // If a single-character string, return the character.
            // Else return the string.
            result_cell_value = (value->s.length() == 1)
                                            ? new CellValue(value->s[0])
                                            : new CellValue(value->s);
            break;
        }

        case NT_NEGATE:
        {
            // Get the NEGATE node's expression node child.
            vector<ICodeNode *> children = node->get_children();
            ICodeNode *expression_node = children[0];

            // Execute the expression and return the negative of its value.
            result_cell_value = execute(expression_node);
            DataValue *value = result_cell_value->value;
            if (value->type == INTEGER) value->i = -value->i;
            else                        value->f = -value->f;

            break;
        }

        case NT_NOT:
        {
            // Get the NOT node's expression node child.
            vector<ICodeNode *> children = node->get_children();
            ICodeNode *expression_node = children[0];

            // Execute the expression and return the "not" of its value.
            result_cell_value = execute(expression_node);
            DataValue *value = result_cell_value->value;
            value->b = !value->b;

            break;
        }

        case NT_CALL:
        {
            // Execute a function call.
            SymTabEntry *function_id =
                                  node->get_attribute((ICodeKey) ID)->id;
            RoutineCode routine_code =
                function_id->get_attribute((SymTabKey) ROUTINE_CODE)
                                                          ->routine_code;
            CallExecutor call_executor(this);
            result_cell_value = call_executor.execute(node);

            // If it was a declared function, obtain the function value
            // from its name.
            if (routine_code == (RoutineCode) RT_DECLARED)
            {
                string function_name = function_id->get_name();
                int nesting_level =
                        function_id->get_symtab()->get_nesting_level();
                ActivationRecord *ar =
                              runtime_stack->get_topmost(nesting_level);
                Cell *function_value_cell = ar->get_cell(function_name);
                CellValue *function_cell_value =
                                        function_value_cell->get_value();

                // Return the function value.
                result_cell_value =
                              new CellValue(function_cell_value->value);

                send_fetch_message(node, function_id->get_name(),
                                   result_cell_value);
            }

            break;
        }

        // Must be a binary operator.
        default: result_cell_value = execute_binary_operator(node,
                                                             node_type);
    }

    return result_cell_value;
}

CellValue *ExpressionExecutor::execute_value(ICodeNode *node)
{
    NodeValue *node_value = node->get_attribute((ICodeKey) ID);
    SymTabEntry *variable_id = node_value->id;
    string variable_name = variable_id->get_name();
    TypeSpec *variable_typespec = variable_id->get_typespec();
    CellValue *result_cell_value;

    // Get the variable's value.
    Cell *variable_cell = execute_variable(node);
    CellValue *variable_cell_value = variable_cell->get_value();

    if (variable_cell_value != nullptr)
    {
        result_cell_value = to_java(
                           variable_typespec, variable_cell_value, node);
    }

    // Uninitialized value error: Use a default value.
    else
    {
        error_handler.flag(node, UNINITIALIZED_VALUE, this);
        result_cell_value =
                        BackendFactory::default_value(variable_typespec);
    }

    send_fetch_message(node, variable_name, result_cell_value);
    return result_cell_value;
}

Cell *ExpressionExecutor::execute_variable(ICodeNode *node)
{
    NodeValue *node_value = node->get_attribute((ICodeKey) ID);
    SymTabEntry *variable_id = node_value->id;
    string variable_name = variable_id->get_name();

    TypeSpec *variable_typespec = variable_id->get_typespec();
    int nesting_level = variable_id->get_symtab()->get_nesting_level();

    // Get the variable reference from the appropriate activation record.
    ActivationRecord *ar = runtime_stack->get_topmost(nesting_level);
    Cell *variable_cell = ar->get_cell(variable_name);
    vector<ICodeNode *> modifiers = node->get_children();

    // Reference to a reference: Use the original reference.
    if (variable_cell->get_value() != nullptr)
    {
        CellValue *variable_cell_value = variable_cell->get_value();

        if (variable_cell_value->cell != nullptr)
        {
            Cell *next_cell = variable_cell_value->cell;
            variable_cell = next_cell;
        }
    }

    // Execute any array subscripts or record fields.
    for (ICodeNode *modifier : modifiers)
    {
        ICodeNodeType node_type = modifier->get_type();

        // Subscripts.
        if (node_type == (ICodeNodeType) NT_SUBSCRIPTS)
        {
            vector<ICodeNode *> subscripts = modifier->get_children();

            // Compute a new reference for each subscript.
            for (ICodeNode *subscript : subscripts)
            {
                TypeValue *type_value =
                    variable_typespec->get_attribute(
                                            (TypeKey) ARRAY_INDEX_TYPE);
                TypeSpec *index_typespec = type_value->typespec;
                int min_index = 0;

                if (index_typespec->get_form() == (TypeForm) TF_SUBRANGE)
                {
                    type_value =
                        index_typespec->get_attribute(
                                          (TypeKey) SUBRANGE_MIN_VALUE);
                    min_index = type_value->value->i;
                }

                CellValue *subscript_cell_value = execute(subscript);
                subscript_cell_value = check_range(node, index_typespec,
                                                   subscript_cell_value);

                int index = subscript_cell_value->value->i - min_index;
                variable_cell =
                          variable_cell->get_value()->cell_array[index];
                delete subscript_cell_value;

                type_value =
                        variable_typespec->get_attribute(
                                          (TypeKey) ARRAY_ELEMENT_TYPE);
                variable_typespec = type_value->typespec;
            }
        }

        // Field.
        else if (node_type == (ICodeNodeType) NT_FIELD)
        {
            node_value = modifier->get_attribute((ICodeKey) ID);
            SymTabEntry *field_id = node_value->id;
            string field_name = field_id->get_name();

            // Compute a new reference for the field.
            MemoryMapImpl *mmap =
                (MemoryMapImpl *) variable_cell->get_value()->memory_map;
            variable_cell = mmap->get_cell(field_name);
            variable_typespec = field_id->get_typespec();
        }
    }

    return variable_cell;
}

CellValue *ExpressionExecutor::execute_binary_operator(
                      ICodeNode *node, const ICodeNodeTypeImpl node_type)
{
    // Get the two operand children of the operator node.
    vector<ICodeNode *> children = node->get_children();
    ICodeNode *operand_node1 = children[0];
    ICodeNode *operand_node2 = children[1];
    // Operands.
    CellValue *cell_value1 = execute(operand_node1);
    CellValue *cell_value2 = execute(operand_node2);
    DataValue *operand1  = cell_value1->value;
    DataValue *operand2  = cell_value2->value;
    TypeSpec  *typespec1 = operand_node1->get_typespec()->base_type();
    TypeSpec  *typespec2 = operand_node2->get_typespec()->base_type();

    CellValue *result_cell_value;

    bool integer_mode = false;
    bool character_mode = false;
    bool string_mode = false;
    bool complex_mode = false; //added

    if (   (typespec1 == Predefined::integer_type)
        && (typespec2 == Predefined::integer_type))
    {
        integer_mode = true;
    }
    //added
    else if ((typespec1 == Predefined::complex_type)
          && (typespec2 == Predefined::complex_type))
    {
        complex_mode = true;
    }

    else if (   (   (typespec1 == Predefined::char_type)
                 || (   (operand1->type == STRING)
                     && (operand1->s.length() == 1)))
             && (   (typespec1 == Predefined::char_type)
                 || (   (operand1->type == STRING)
                     && (operand1->s.length() == 1))))
    {
        character_mode = true;
    }
    else if (   (operand1->type == STRING)
             && (operand2->type == STRING))
    {
        string_mode = true;
    }

    // ====================
    // Arithmetic operators
    // ====================

    if (ARITH_OPS.find(node_type) != ARITH_OPS.end())
    {
        if (integer_mode)
        {
            int value1 = operand1->i;
            int value2 = operand2->i;

            // Integer operations.
            switch (node_type)
            {
                case NT_ADD:
                {
                    result_cell_value = new CellValue(value1 + value2);
                    break;
                }

                case NT_SUBTRACT:
                {
                    result_cell_value = new CellValue(value1 - value2);
                    break;
                }

                case NT_MULTIPLY:
                {
                    result_cell_value = new CellValue(value1 * value2);
                    break;
                }

                case NT_FLOAT_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        result_cell_value = new CellValue(((float) value1) /
                                                          ((float) value2));
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        result_cell_value = new CellValue(0.0f);
                    }

                    break;
                }

                case NT_INTEGER_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        result_cell_value = new CellValue(value1/value2);
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        result_cell_value = new CellValue(0);
                    }

                    break;
                }

                case NT_MOD:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        result_cell_value = new CellValue(value1%value2);
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        result_cell_value = new CellValue(0);
                    }

                    break;
                }

                default: result_cell_value = nullptr;  // shouldn't get here
            }
        }
        //added. put as first if statement
        else if (complex_mode)
        {
            /*
            //retrieving the values
            TypeValue *value1 = typespec1->get_attribute((TypeKey) RECORD_SYMTAB);
            TypeValue *value2 = typespec2->get_attribute((TypeKey) RECORD_SYMTAB);

            //getting the symbol tables
            SymTab *table1 = value1->symtab;
            SymTab *table2 = value2->symtab;

            SymTabEntry *re1 = table1->lookup("re");
            SymTabEntry *im1 = table1->lookup("im");
            SymTabEntry *re2 = table2->lookup("re");
            SymTabEntry *im2 = table2->lookup("im");

            //how do I retrieve the value from
            //the SymTabEntry object?
            //I believe I need to get the EntryValue from
            //SymTab object and then retrieve the DataValue
            //from that. Finally retrieve the float associated
            //with that DataValue
            EntryValue *val_re1 = re1->get_attribute((SymTabKey) DATA_VALUE);
            EntryValue *val_im1 = im1->get_attribute((SymTabKey) DATA_VALUE);
            EntryValue *val_re2 = re2->get_attribute((SymTabKey) DATA_VALUE);
            EntryValue *val_im2 = im2->get_attribute((SymTabKey) DATA_VALUE);

            DataValue *dval_re1 = val_re1->value;
            DataValue *dval_im1 = val_im1->value;
            DataValue *dval_re2 = val_re2->value;
            DataValue *dval_im2 = val_im2->value;

            float true_re1 = dval_re1->f;
            float true_im1 = dval_im1->f;
            float true_re2 = dval_re2->f;
            float true_im2 = dval_im2->f;
            */


            //this is where we actually start working
            MemoryMap *map1 = cell_value1->memory_map;
            MemoryMap *map2 = cell_value2->memory_map;

            Cell *cell_re_1 = map1->get_cell("re");
            Cell *cell_im_1 = map1->get_cell("im");
            Cell *cell_re_2 = map2->get_cell("re");
            Cell *cell_im_2 = map2->get_cell("im");

            CellValue *cellValue_re_1 = cell_re_1->get_value();
            CellValue *cellValue_im_1 = cell_im_1->get_value();
            CellValue *cellValue_re_2 = cell_re_2->get_value();
            CellValue *cellValue_im_2 = cell_im_2->get_value();

            DataValue *dvalue_re_1 = cellValue_re_1->value;
            DataValue *dvalue_im_1 = cellValue_im_1->value;
            DataValue *dvalue_re_2 = cellValue_re_2->value;
            DataValue *dvalue_im_2 = cellValue_im_2->value;

            float true_re1 = dvalue_re_1->f;
            float true_im1 = dvalue_im_1->f;
            float true_re2 = dvalue_re_2->f;
            float true_im2 = dvalue_im_2->f;
            MemoryMapImpl *sum_map = new MemoryMapImpl; //changed to Impl


            switch (node_type)
            {
                case NT_ADD:
                {
                  /*
                    float sum_re = true_re1 + true_re2;
                    float sum_im = true_im1 + true_im2;
                    //DataValue *dsum_re = new DataValue(sum_re);
                    //DataValue *dsum_im = new DataValue(sum_im);
                    CellValue *csum_re = new CellValue(sum_re);
                    CellValue *csum_im = new CellValue(sum_im);
                    CellImpl *cell_sum_re = new CellImpl(csum_re); //changed to Impl
                    CellImpl *cell_sum_im = new CellImpl(csum_im); //changed to Impl
                    //we need to make these cells point go into
                    //a memory map and then make result_cell_value
                    //point to the memory map and return it
                    sum_map->contents.insert("re", cell_sum_re);
                    sum_map->contents.insert("im", cell_sum_im);
                    */

                    break;
                }
                case NT_SUBTRACT:
                {
                    float sum_re = true_re1 - true_re2;
                    float sum_im = true_im1 - true_im2;
                    break;
                }
                case NT_MULTIPLY:
                {
                    float sum_re = (true_re1 * true_re2) - (true_im1 * true_im2);
                    float sum_im = (true_re1 * true_im2) + (true_im1 * true_re2);
                    break;
                }
                case NT_FLOAT_DIVIDE:
                {
                    float sum_re = ((true_re1 * true_re2) + (true_im1 * true_im2)) / ((true_re2 * true_re2) + (true_im2 * true_im2));
                    float sum_im = ((true_im1 * true_re2) - (true_re1 * true_im2)) / ((true_re2 * true_re2) + (true_im2 * true_im2));
                    break;
                }
                case NT_INTEGER_DIVIDE:
                {
                    float sum_re = ((true_re1 * true_re2) + (true_im1 * true_im2)) / ((true_re2 * true_re2) + (true_im2 * true_im2));
                    float sum_im = ((true_im1 * true_re2) - (true_re1 * true_im2)) / ((true_re2 * true_re2) + (true_im2 * true_im2));
                    break;
                }
                default: result_cell_value = nullptr; //shouldn't get here
            }

        }
        else
        {
            float value1 = operand1->type == INTEGER ? operand1->i
                                                     : operand1->f;
            float value2 = operand2->type == INTEGER ? operand2->i
                                                     : operand2->f;

            // Float operations.
            switch (node_type)
            {
                case NT_ADD:
                {
                    result_cell_value = new CellValue(value1 + value2);
                    break;
                }

                case NT_SUBTRACT:
                {
                    result_cell_value = new CellValue(value1 - value2);
                    break;
                }

                case NT_MULTIPLY:
                {
                    result_cell_value = new CellValue(value1 * value2);
                    break;
                }

                case NT_FLOAT_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0.0f)
                    {
                        result_cell_value = new CellValue(value1/value2);
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        result_cell_value = new CellValue(0.0f);
                    }

                    break;
                }

                default: result_cell_value = nullptr;  // shouldn't get here
            }
        }
    }

    // ==========
    // AND and OR
    // ==========

    else if ((node_type == NT_AND) || (node_type == NT_OR))
    {
        bool value1 = operand1->b;
        bool value2 = operand2->b;

        switch (node_type)
        {
            case NT_AND:
            {
                result_cell_value = new CellValue(value1 && value2);
                break;
            }

            case NT_OR:
            {
                result_cell_value = new CellValue(value1 || value2);
                break;
            }

            default: result_cell_value = nullptr;  // shouldn't get here
        }
    }

    // ====================
    // Relational operators
    // ====================

    else if (integer_mode)
    {
        int value1 = operand1->i;
        int value2 = operand2->i;

        // Integer operands.
        switch (node_type)
        {
            case NT_EQ:
            {
                result_cell_value = new CellValue(value1 == value2);
                break;
            }

            case NT_NE:
            {
                result_cell_value = new CellValue(value1 != value2);
                break;
            }

            case NT_LT:
                result_cell_value = new CellValue(value1 < value2);
                break;

            case NT_LE:
            {
                result_cell_value = new CellValue(value1 <= value2);
                break;
            }

            case NT_GT:
            {
                result_cell_value = new CellValue(value1 > value2);
                break;
            }

            case NT_GE:
            {
                result_cell_value = new CellValue(value1 >= value2);
                break;
            }

            default: result_cell_value = nullptr;  // shouldn't get here
        }
    }
    else if (character_mode)
    {
        int value1 = operand1->type == CHAR ? operand1->c
                                            : operand1->s[0];
        int value2 = operand2->type == CHAR ? operand2->c
                                            : operand2->s[0];

        // Character operands.
        switch (node_type)
        {
            case NT_EQ:
            {
                result_cell_value = new CellValue(value1 == value2);
                break;
            }

            case NT_NE:
            {
                result_cell_value = new CellValue(value1 != value2);
                break;
            }

            case NT_LT:
                result_cell_value = new CellValue(value1 < value2);
                break;

            case NT_LE:
            {
                result_cell_value = new CellValue(value1 <= value2);
                break;
            }

            case NT_GT:
            {
                result_cell_value = new CellValue(value1 > value2);
                break;
            }

            case NT_GE:
            {
                result_cell_value = new CellValue(value1 >= value2);
                break;
            }

            default: result_cell_value = nullptr;  // shouldn't get here
        }
    }
    else if (string_mode)
    {
        string value1 = operand1->s;
        string value2 = operand2->s;

        // String operands.
        switch (node_type)
        {
            case NT_EQ:
            {
                result_cell_value = new CellValue(value1 == value2);
                break;
            }

            case NT_NE:
            {
                result_cell_value = new CellValue(value1 != value2);
                break;
            }

            case NT_LT:
                result_cell_value = new CellValue(value1 < value2);
                break;

            case NT_LE:
            {
                result_cell_value = new CellValue(value1 <= value2);
                break;
            }

            case NT_GT:
            {
                result_cell_value = new CellValue(value1 > value2);
                break;
            }

            case NT_GE:
            {
                result_cell_value = new CellValue(value1 >= value2);
                break;
            }

            default: result_cell_value = nullptr;  // shouldn't get here
        }
    }
    else
    {
        float value1 = operand1->type == INTEGER ? operand1->i
                                                 : operand1->f;
        float value2 = operand2->type == INTEGER ? operand2->i
                                                 : operand2->f;

        // Float operands.
        switch (node_type)
        {
            case NT_EQ:
            {
                result_cell_value = new CellValue(value1 == value2);
                break;
            }

            case NT_NE:
            {
                result_cell_value = new CellValue(value1 != value2);
                break;
            }

            case NT_LT:
                result_cell_value = new CellValue(value1 < value2);
                break;

            case NT_LE:
            {
                result_cell_value = new CellValue(value1 <= value2);
                break;
            }

            case NT_GT:
            {
                result_cell_value = new CellValue(value1 > value2);
                break;
            }

            case NT_GE:
            {
                result_cell_value = new CellValue(value1 >= value2);
                break;
            }

            default: result_cell_value = nullptr;  // shouldn't get here
        }
    }

    delete cell_value1;
    delete cell_value2;

    return result_cell_value;
}

}}}}  // namespace wci::backend::interpreter::executors
