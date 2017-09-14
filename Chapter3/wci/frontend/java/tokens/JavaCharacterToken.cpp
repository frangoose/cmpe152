/*
 * JavaCharacterToken.cpp
 *
 *  Created on: Sep 13, 2017
 *      Author: Nick
 */

#include <iostream>
#include <string>
#include "JavaCharacterToken.h"
#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaCharacterToken::JavaCharacterToken(Source *source) throw (string)
		: JavaToken(source)
{
	extract();
}

/*
 * Extract a java character token from the source.
 * throws exception if an error occured
 */

void JavaCharacterToken::extract() throw (string)
{
	string value_str = "";
	char current_ch = next_char(); //consume initial single quote
	text += "\'";

	/*
	 * posibilites are:
	 * a letter
	 * a \'
	 * a \\
	 * a \t
	 * a \n
	 */

	// if its a letter
	if (isalpha(current_ch))
	{
		text += current_ch;
		value_str += current_ch;
		current_ch = next_char(); //consume the letter
	}

	else if (current_ch == '\\')
	{
		current_ch = next_char(); // consume the slash

		//this means it's a double slash
		if(current_ch == '\\')
		{
			text += "\\\\";
			value_str += "\\";
			current_ch = next_char(); //consume the second slash
		}
		//this means it's a tab character
		else if (current_ch == 't')
		{
			text += "\\t";
			value_str += "\t";
			current_ch = next_char(); //consume the t;
		}
		//this means it's a newline character
		else if (current_ch == 'n')
		{
			text += "\\n";
			value_str += "\n";
			current_ch = next_char(); //consume the n
		}
		//this means it's a single quote character
		else if (current_ch == '\'')
		{
			text += "\\'";
			value_str += "\'";
			current_ch = next_char(); //consume the single quote
		}
	}
	//if it's the end of the CHAR
	if (current_ch == '\'')
	{
		next_char(); //consume the final single quote
		text += '\'';
		type = (TokenType) JT_CHARACTER;
		value = new DataValue(value_str);
	}

	//else it's an error
	else
	{
		type = (TokenType) JT_ERROR;
		value = new DataValue((int) UNEXPECTED_EOF);
	}
}

}}}} // namespace wci::frontend::java::tokens
