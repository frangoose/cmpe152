/*
 * JavaSpecialSymbolToken.cpp
 *
 *  Created on: Sep 13, 2017
 *      Author: Nick
 */

#include <string>
#include "JavaSpecialSymbolToken.h"
#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaSpecialSymbolToken::JavaSpecialSymbolToken(Source *source) throw (string)
		: JavaToken(source)
{
	extract();
}

void JavaSpecialSymbolToken::extract() throw (string)
{

	char current_ch = current_char();
	bool good_symbol = true;

	text = current_ch;

	// a lot of this code could be shortened, but I wanted
	// to make sure that I covered everything I needed to
	switch(current_ch)
	{
		//single-character special symbols
	case '~': case '@': case ':': case ';': case '?':
	case '.': case ',': case '\'': case '\"':
	case '(': case ')': case '[': case ']': case '{':
	case '}':
	{
		next_char(); //consume the single character
		break; //to stop here
	}

	//now we're going to check for all the two character special symbols
	// + or ++ or +=
	case '+':
	{
		current_ch = next_char(); //consume '+';

		if (current_ch == '+')
		{
			text += current_ch;
			next_char(); //consume '+'
		}
		else if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		break;
	}
	// - or -- or -=
	case '-':
	{
		current_ch = next_char(); //consume '-';

		if (current_ch == '-')
		{
			text += current_ch;
			next_char(); //consume '-'
		}
		else if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='

		break;
	}
	}

	//< or << or <= or <<=
	case '<':
	{
		current_ch = next_char(); //consume '<'

		//<=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		//<<=
		else if ((current_ch == '<') && (peek_char() == '='))
		{
			text += current_ch;
			current_ch = next_char(); //consume '<'
			text += current_ch;
			next_char(); //consume '='
		}

		// <<
		else if (current_ch == '<')
		{
			text += current_ch;
			next_char(); //consume '<'
		}

		break;
	}

	case '>':
	{
		current_ch = next_char(); //consume '>'

		//>=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		//>>=
		else if ((current_ch == '>') && (peek_char() == '='))
		{
			text += current_ch;
			current_ch = next_char(); //consume '>'
			text += current_ch;
			next_char(); //consume '='
		}

		// >>
		else if (current_ch == '>')
		{
			text += current_ch;
			next_char(); //consume '>'
		}

		break;
	}
	// ! or !=
	case '!':
	{
		current_ch = next_char(); //consume '!'

		//!=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		break;
	}
	// % or %=
	case '%':
	{
		current_ch = next_char(); //consume '%'

		//%=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		break;
	}
	// ^ or ^=
	case '^':
	{
		current_ch = next_char(); //consume '^'

		//^=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		break;
	}
	//* or *= or */
	case '*':
	{
		current_ch = next_char(); //consume '*'

		//*=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}
		// */
		else if (current_ch == '/')
		{
			text += current_ch;
			next_char(); //consume '/'
		}

		break;
	}
	// = or ==
	case '=':
	{
		current_ch = next_char(); //consume '='

		//==
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}

		break;
	}
	//& or &= or &&
	case '&':
	{
		current_ch = next_char(); //consume '&'

		//&=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}
		//&&
		else if (current_ch == '&')
		{
			text += current_ch;
			next_char(); //consume '&'
		}

		break;
	}
	// | or |= or ||
	case '|':
	{
		current_ch = next_char(); //consume '|'

		// |=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}
		// ||
		else if (current_ch == '|')
		{
			text += current_ch;
			next_char(); //consume '|'
		}

		break;
	}
	// / or /= or // or /*
	case '/':
	{
		current_ch = next_char(); //consume '/'

		// /=
		if (current_ch == '=')
		{
			text += current_ch;
			next_char(); //consume '='
		}
		// //
		else if (current_ch == '/')
		{
			text += current_ch;
			next_char(); //consume '/'
		}
		// /*
		else if (current_ch == '*')
		{
			text += current_ch;
			next_char(); //consume '*'
		}

		break;
	}

	default:
	{
		next_char(); //consume bad character
		type = (TokenType) (JT_ERROR);
		value = new DataValue((int) INVALID_CHARACTER);
		good_symbol = false;
	}

	}//end of switch

	// Set the type if it wasn't an error
	if (good_symbol) {
		type = (TokenType) (JavaToken::SPECIAL_SYMBOLS[text]);
	}
}

}}}} // namespace wci::frontend::java::tokens
