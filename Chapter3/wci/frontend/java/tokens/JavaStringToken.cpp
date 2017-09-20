/*
 * JavaStringToken.cpp
 *
 *  Created on: Sep 13, 2017
 *      Author: Nick
 */

#include <string>
#include "JavaStringToken.h"
#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaStringToken::JavaStringToken(Source *source) throw (string)
		: JavaToken(source)
{
	extract();
}

void JavaStringToken::extract() throw (string)
{

	string value_str = "";

	char current_ch = next_char(); //consume initial quote
	text += "\"";

	//Get string characters

	//three if statements in this do while loop to determine what to
	//do with characters
	do
	{

		//Replace any whitespace character with a blank
		if (isspace(current_ch)) current_ch = ' ';

		//this means it's a part of the string
		if ((current_ch != '\\') && (current_ch != '\"')
				&& (current_ch != EOF))
		{
			text += current_ch;
			value_str += current_ch;
			current_ch = next_char(); //consume character
		}

		//is it the '\' character? that means we have to do
		//something special here
		//if it is we need to worry about if the followed character
		//is a 't' or 'n' or '"'
		if (current_ch == '\\')
		{
			current_ch = next_char(); //consume the initial slash

			if (current_ch == 't')
			{
				text += "\\t";
				value_str += "\t";
				current_ch = next_char(); //consume the 't'
			}

			else if (current_ch == 'n')
			{
				text += "\\n";
				value_str += "\n";
				current_ch = next_char(); //consume the 'n'
			}

			else if (current_ch == '\"')
			{
				text += "\\";
				text += "\"";
				value_str += current_ch; //append the double quotes
				current_ch = next_char(); //consume the quote
			}
		}

	} while ((current_ch != '\"') && (current_ch != Source::END_OF_FILE));

	//if end of string
	if (current_ch == '\"')
	{
		next_char(); //consume final quote
		text += '\"';
		type = (TokenType) JT_STRING;
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
