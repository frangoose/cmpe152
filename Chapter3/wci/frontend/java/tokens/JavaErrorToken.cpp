/*
 * JavaErrorToken.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: Nick
 */

#include "JavaErrorToken.h"
#include "../../Token.h"
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

JavaErrorToken::JavaErrorToken(Source *source, JavaErrorCode error_code,
                                   string token_text)
    throw (string)
    : JavaToken(source)
{
    type = (TokenType) JT_ERROR;
    text = token_text;
    value = new DataValue((int) error_code);
}

void JavaErrorToken::extract() throw (string)
{
    // do nothing
}

}}}}  // namespace wci::frontend::Java::tokens
