/*
 * JavaScanner.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: Nick
 */

#include <iostream>
#include "JavaScanner.h"
#include "JavaToken.h"
#include "JavaError.h"
#include "../Source.h"
#include "tokens/JavaWordToken.h"
#include "tokens/JavaNumberToken.h"
#include "tokens/JavaStringToken.h"
#include "tokens/JavaSpecialSymbolToken.h"
#include "tokens/JavaCharacterToken.h"
#include "tokens/JavaErrorToken.h"

namespace wci { namespace frontend { namespace java {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java::tokens;

JavaScanner::JavaScanner(Source *source) : Scanner(source)
{
}

Token *JavaScanner::extract_token() throw (string)
{
	skip_white_space();

	Token *token;
	char current_ch = current_char();
	string string_ch = "";

	string_ch[0] = current_ch;

	//Construct the next token. The current character determines
	//the token type

	if (current_ch == Source::END_OF_FILE)
	{
		token = nullptr;
	}

	else if (isalpha(current_ch))
	{
		token = new JavaWordToken(source);
	}

	else if (isdigit(current_ch))
	{
		token = new JavaNumberToken(source);
	}

	//if it's a double quote character create string token
	else if (current_ch == '\"')
	{
		token = new JavaStringToken(source);
	}

	//if it's a single quote character create a character token
	else if (current_ch == '\'')
	{
		token = new JavaCharacterToken(source);
	}

	//if it's a special symbol
	else if (JavaToken::SPECIAL_SYMBOLS.find(string_ch)
				== JavaToken::SPECIAL_SYMBOLS.end())
	{
		token = new JavaSpecialSymbolToken(source);
	}
	//else it's an error token
	else
	{	//cout << "error token here in scanner" << endl;
		token = new JavaErrorToken(source, INVALID_CHARACTER, string_ch);
		next_char(); //consume current character
	}

	return token;
}

void JavaScanner::skip_white_space() throw(string)
{ //1
	char current_ch = current_char();

	while (isspace(current_ch) || (current_ch == '/'))
	{ //2
		//find out if it's a comment
		if (current_ch == '/')
		{ //3
			current_ch = next_char();
			
			//it's a comment if this executes
			if ((current_ch == '/') || (current_ch == '*'))
			{//4
				//single line comment
				if (current_ch == '/')
				{
					do{
						current_ch = next_char();
					}while((current_ch != '\n'));

				}
				//block comment
				else if (current_ch == '*')
				{
					bool n = true;
					do
					{
						current_ch = next_char();
						if(current_ch == '*')
						{
							current_ch = next_char();
							if(current_ch == '/')
							{
								n = false;
								current_ch = next_char(); //consume character
							}
						}
					}while(n);
				}
			}//4

		}//3
		
		//it's whitespace
		else current_ch = next_char();
	} //2

} //1

}}}
