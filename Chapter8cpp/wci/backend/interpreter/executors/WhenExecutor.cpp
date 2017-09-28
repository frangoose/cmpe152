#include <vector>
#include "WhenExecutor.h"
#include "StatementExecutor.h"
#include "ExpressionExecutor.h"
#include "../../../DataValue.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

WhenExecutor::WhenExecutor(Executor *parent)
      : StatementExecutor(parent)
{
}

DataValue *WhenExecutor::execute(ICodeNode *node)
{
    //get the when node's children. the vector will contain
    //the when_branch nodes and the otherwise node
    vector<ICodeNode *> children = node->get_children();
    //test condition to control exit of loop if we execute a
    //when branches statement expression
    bool when_test = true;
    //creating variables to make sure the while loop doesn't try to
    //execute when we've went through the whole parse tree
    int size_when = children.size();
    int current = 0;

    while (when_test && (current < size_when - 2))
    {
          ICodeNode *when_branch = children[current];

          //create the executors
          ExpressionExecutor expression_executor(this);
          StatementExecutor statement_executor(this);

          current++; //increment current
    }

    //if when_test is still true that means that we checked
    //each when branch and they were all false
    //so we need to execute the otherwise branch
    if (when_test)
    {
    //creating the otherwise_node it will be the last child of when
    ICodeNode *otherwise_node = children[size_when-1];

    //executing the statement node of otherwise
    ICodeNode *statement_node = otherwise_node->get_children()[0];
    statement_executor.execute(statement_node);
    }
}

}}}}
