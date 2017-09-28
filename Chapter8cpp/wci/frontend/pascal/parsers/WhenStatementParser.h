#ifndef WHENSTATEMENTPARSER_H
#define WHENSTATEMENTPARSER_H

#include <string>
#include <set>
#include "StatementParser.h"
#include "../../Token.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class WhenStatementParser : public StatementParser
{
public:
  /*
  constructor
  parent the parent parser
  */

  WhenStatementParser(PascalParserTD *parent);

  /*
  Parse a WHEN statement
  parameter token is the inital token
  return the root node of the generated parse tree
  throw a string message if an error occured
  */

  ICodeNode *parse_statement(Token *token) throw (string);

private:
  //synchronization set for RIGHT_ARROW
  static set<PascalTokenType> RIGHT_ARROW_SET;

  static bool INITIALIZED;

  //initialize the synchronization set
  static void initialize();
 }

}}}} //namespace wci::frontend::pascal::parsers

#endif  // WHENSTATEMENTPARSER_H
