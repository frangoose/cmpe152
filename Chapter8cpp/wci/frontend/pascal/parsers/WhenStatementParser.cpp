#include <string>
#include <set>
#include "WhenStatementParser.h"
#include "AssignmentStatementParser.h"
#include "ExpressionParser.h"
#include "../PascalParserTD.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../Token.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

bool WhenStatementParser::INITIALIZED = false;

set <PascalTokenType> WhenStatementParser::RIGHT_ARROW_SET;

void WhenStatementParser::initialize()
{
  if (INITIALIZED) return;

  RIGHT_ARROW_SET = StatementParser::STMT_START_SET;
  RIGHT_ARROW_SET.insert(PascalTokenType::RIGHT_ARROW);

  set<PascalTokenType>::iterator it;
  for (it = StatementParser::STMT_FOLLOW_SET.begin();
        it != StatementParser::STMT_FOLLOW_SET.end();
        it++)
  {
        RIGHT_ARROW_SET.insert(*it);
  }

  INITIALIZED = true;
}

WhenStatementParser::WhenStatementParser(PascalParserTD *parent)
      : StatementParser(parent)
{
  initialize();
}

ICodeNode *WhenStatementParser::parse_statement(Token *token) throw (string)
{
    token = next_token(token); //consume the WHEN


    // create a WHEN node
    ICodeNode *when_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);

    //parse the WHEN


    //Loop to parse each WHEN branch until the END token
    //or the end of the source file
    while ((token != nullptr) &&
          (token->get_type() != (TokenType) PT_END) &&
           (token->get_type() != (TokenType) PT_OTHERWISE))
    {
          //the WHEN node adopts the WHEN branch subtree
          when_node->add_child(parse_branch(token));

          token = current_token();
          TokenType token_type = token->get_type();

          //look for the semicolon between the WHEN branches
          if (token_type == (TokenType) PT_SEMICOLON)
          {
            token = next_token(token); //consume the ;
          }

          //if at the start of the next WHEN branch, then mission a semicolon
          else
          {
            error_handler.flag(token, MISSING_SEMICOLON, this);
          }
    }

    //look for the OTHERWISE token
    if (token->get_type() == (TokenType) PT_OTHERWISE)
    {
      token = next_token(token); //consume OTHERWISE
      //now we need to parse the otherwise statement and add it to the when node
      //Synchronize at the RIGHT_ARROW
      token = synchronize(RIGHT_ARROW_SET);
      if (token->get_type() == (TokenType) PT_RIGHT_ARROW)
      {
        token = next_token(token); //consume the RIGHT_ARROW
      }
      else {
        error_handler.flag(token, MISSING_RIGHT_ARROW, this);
      }

      //just need to parse assignment statement now
      StatementParser statementParser(this);
      //the statement subtree is added as the last child of the
      //when_node
      when_node->add_child(statementParser.parse_statement(token));

    }
    else{
      error_handler.flag(token, MISSING_OTHERWISE, this);
    }

    //look for the END token
    if (token->get_type() == (TokenType) PT_END)
    {
      token = next_token(token); //consume END
    }
    else
    {
      error_handler.flag(token, MISSING_END, this);
    }
}

ICodeNode *WhenStatementParser::parse_branch(Token *token) throw (string)
{
  //create a WHEN_BRANCH node EQ and ASSIGN node
  //the WHEN_BRANCH adopts the EQ node as it's first child
  //and the ASSIGN branch as it's second child
  ICodeNode *branch_node =
            ICodeFactory::create_icode_node(
                  (ICodeNodeType) NT_WHEN_BRANCH);
  //parse the expression
  // the branch_node adopts the expression subtree as it's first child
  ExpressionParser expression_parser(this);
  branch_node->add_child(expression_parser.parse_statement(token));

  //Synchronize at the RIGHT_ARROW
  token = synchronize(RIGHT_ARROW_SET);
  if (token->get_type() == (TokenType) PT_RIGHT_ARROW)
  {
    token = next_token(token); //consume the RIGHT_ARROW
  }
  else {
    error_handler.flag(token, MISSING_RIGHT_ARROW, this);
  }

  //parse the when_branch statement. The branch_node adopts
  //the statement subtree as its second child
  StatementParser statementParser(this);
  branch_node->add_child(statementParser.parse_statement(token));

  return branch_node;

}}}}
