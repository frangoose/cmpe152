#ifndef WHENEXECUTOR_H
#define WHENEXECUTOR_H

#include "StatementExecutor.h"
#include "../../../DataValue.h"
#include "../../../intermediate/ICodeNode.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;

class WhenExecutor : public StatementExecutor
{
public:

  //Constructor
  WhenExecutor(Executor *parent);

  //execute a when statement
  //parameter node is the root node of the when statement
  //return nullptr
  DataValue *execute(ICodeNode *node);
};

}}}} //namespace wci::backend::interpreter::executors

#endif // WHENEXECUTOR_H
