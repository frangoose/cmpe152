/*
 * JavaWordToken.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: Nick
 */

#include <iostream>
#include <string>
#include <map>
#include "JavaWordToken.h"
#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaWordToken::JavaWordToken(Source *source) throw (string)
		: JavaToken(source)
{
	extract();
}

/*
 * Extract a java word token from he source
 * throws exception if an error occurred
 */

void JavaWordToken::extract() throw (string)
{

    char current_ch = current_char();

    // Get the word characters (letter or digit). The scanner has
    // already determined that the first character is a letter.
    while (isalnum(current_ch))
    {
        text += current_ch;
        current_ch = next_char();  // consume character
    }

    if (JavaToken::RESERVED_WORDS.find(text)
            != JavaToken::RESERVED_WORDS.end())
    {
        // Reserved word.
        type = (TokenType) JavaToken::RESERVED_WORDS[text];
        value = new DataValue(text);
    }
    else
    {
        // Identifier.
        type = (TokenType) JT_IDENTIFIER;
    }
}

}}}} //namespace wci::frontend::java::tokens
