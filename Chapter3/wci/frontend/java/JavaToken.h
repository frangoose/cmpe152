/*
 * JavaToken.h
 *
 *  Created on: Sep 10, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_JAVATOKEN_H_
#define WCI_FRONTEND_JAVA_JAVATOKEN_H_

#include <string>
#include <map>
#include "../Token.h"
#include "../Source.h"

namespace wci { namespace frontend { namespace java {

using namespace std;
using namespace wci::frontend;

/*
 * Java token types.
 */

enum class JavaTokenType
{
    // Reserved words.
    ABSTRACT, DOUB, INT, LONG, BREAK, ELSE, SWITCH, CASE, ENUM,
	NATIVE, SUPER, CHAR, EXTENDS, RETURN, THIS, CLASS, FLOAT, SHORT,
	THROW, CONST, FOR, PACKAGE, VOID, CONTINUE, GOTO, PROTECTED, VOLATILE,
	DO, IF, STATIC, WHILE,

    // Special symbols.
    TILDE, EXCLAMATION, AT, MOD, XOR, AMPERSAND, MULTIPLY,
	MINUS, ADD, ASSIGN, PIPE, DIVIDE, COLON, SEMICOLON,
	QUESTION, LESS_THAN, GREATER_THAN, DOT, COMMA, SINGLE_QUOTE,
	DOUBLE_QUOTE, OPEN_PAR, CLOSE_PAR, OPEN_BRACK, CLOSE_BRACK, OPEN_CURLY,
	CLOSE_CURLY, INCREMENT, DECREMENT, LESS_LESS_THAN, GREAT_GREAT_THAN,
	LESS_THAN_EQUALS, GREAT_THAN_EQUALS, ADD_EQUALS, MINUS_EQUALS,
	MULT_EQUALS, DIVIDEEQUALS, EQUALS, PIPEEQUALS, MOD_EQUALS,
	AMP_EQUALS, XOR_EQUALS, EXCLAM_EQUALS, LESS_LESS_EQUALS,
	GREAT_GREAT_EQUALS, OR, AND, COMMENT, BLOCK_COMMENT_OPEN, BLOCK_COMMENT_CLOSE,

    IDENTIFIER, INTEGER, DOUBLE, STRING,
    ERROR, END_OF_FILE,
};

//RESERVED WORDS
constexpr JavaTokenType JT_ABSTRACT = JavaTokenType::ABSTRACT;
constexpr JavaTokenType JT_DOUB = JavaTokenType::DOUB;
constexpr JavaTokenType JT_INT = JavaTokenType::INT;
constexpr JavaTokenType JT_LONG = JavaTokenType::LONG;
constexpr JavaTokenType JT_BREAK = JavaTokenType::BREAK;
constexpr JavaTokenType JT_ELSE = JavaTokenType::ELSE;
constexpr JavaTokenType JT_SWITCH = JavaTokenType::SWITCH;
constexpr JavaTokenType JT_CASE = JavaTokenType::CASE;
constexpr JavaTokenType JT_ENUM = JavaTokenType::ENUM;
constexpr JavaTokenType JT_NATIVE = JavaTokenType::NATIVE;
constexpr JavaTokenType JT_SUPER = JavaTokenType::SUPER;
constexpr JavaTokenType JT_CHAR = JavaTokenType::CHAR;
constexpr JavaTokenType JT_EXTENDS = JavaTokenType::EXTENDS;
constexpr JavaTokenType JT_RETURN = JavaTokenType::RETURN;
constexpr JavaTokenType JT_THIS = JavaTokenType::THIS;
constexpr JavaTokenType JT_CLASS = JavaTokenType::CLASS;
constexpr JavaTokenType JT_FLOAT = JavaTokenType::FLOAT;
constexpr JavaTokenType JT_SHORT = JavaTokenType::SHORT;
constexpr JavaTokenType JT_THROW = JavaTokenType::THROW;
constexpr JavaTokenType JT_CONST = JavaTokenType::CONST;
constexpr JavaTokenType JT_FOR = JavaTokenType::FOR;
constexpr JavaTokenType JT_PACKAGE = JavaTokenType::PACKAGE;
constexpr JavaTokenType JT_VOID = JavaTokenType::VOID;
constexpr JavaTokenType JT_CONTINUE = JavaTokenType::CONTINUE;
constexpr JavaTokenType JT_GOTO = JavaTokenType::GOTO;
constexpr JavaTokenType JT_PROTECTED = JavaTokenType::PROTECTED;
constexpr JavaTokenType JT_VOLATILE = JavaTokenType::VOLATILE;
constexpr JavaTokenType JT_DO = JavaTokenType::DO;
constexpr JavaTokenType JT_IF = JavaTokenType::IF;
constexpr JavaTokenType JT_STATIC = JavaTokenType::STATIC;
constexpr JavaTokenType JT_WHILE = JavaTokenType::WHILE;


//SPECIAL SYMBOLS

constexpr JavaTokenType JT_TILDE = JavaTokenType::TILDE;
constexpr JavaTokenType JT_EXCLAMATION = JavaTokenType::EXCLAMATION;
constexpr JavaTokenType JT_AT = JavaTokenType::AT;
constexpr JavaTokenType JT_MOD = JavaTokenType::MOD;
constexpr JavaTokenType JT_XOR = JavaTokenType::XOR;
constexpr JavaTokenType JT_AMPERSAND = JavaTokenType::AMPERSAND;
constexpr JavaTokenType JT_MULTIPLY = JavaTokenType::MULTIPLY;
constexpr JavaTokenType JT_MINUS = JavaTokenType::MINUS;
constexpr JavaTokenType JT_ADD = JavaTokenType::ADD;
constexpr JavaTokenType JT_ASSIGN = JavaTokenType::ASSIGN;
constexpr JavaTokenType JT_PIPE = JavaTokenType::PIPE;
constexpr JavaTokenType JT_DIVIDE = JavaTokenType::DIVIDE;
constexpr JavaTokenType JT_COLON = JavaTokenType::COLON;
constexpr JavaTokenType JT_SEMICOLON = JavaTokenType::SEMICOLON;
constexpr JavaTokenType JT_QUESTION = JavaTokenType::QUESTION;
constexpr JavaTokenType JT_LESS_THAN = JavaTokenType::LESS_THAN;
constexpr JavaTokenType JT_GREATER_THAN = JavaTokenType::GREATER_THAN;
constexpr JavaTokenType JT_DOT = JavaTokenType::DOT;
constexpr JavaTokenType JT_COMMA = JavaTokenType::COMMA;
constexpr JavaTokenType JT_SINGLE_QUOTE = JavaTokenType::SINGLE_QUOTE;
constexpr JavaTokenType JT_DOUBLE_QUOTE = JavaTokenType::DOUBLE_QUOTE;
constexpr JavaTokenType JT_OPEN_PAR = JavaTokenType::OPEN_PAR;
constexpr JavaTokenType JT_CLOSE_PAR = JavaTokenType::CLOSE_PAR;
constexpr JavaTokenType JT_OPEN_BRACK = JavaTokenType::OPEN_BRACK;
constexpr JavaTokenType JT_CLOSE_BRACK = JavaTokenType::CLOSE_BRACK;
constexpr JavaTokenType JT_OPEN_CURLY = JavaTokenType::OPEN_CURLY;
constexpr JavaTokenType JT_CLOSE_CURLY = JavaTokenType::CLOSE_CURLY;
constexpr JavaTokenType JT_INCREMENT = JavaTokenType::INCREMENT;
constexpr JavaTokenType JT_DECREMENT = JavaTokenType::DECREMENT;
constexpr JavaTokenType JT_LESS_LESS_THAN = JavaTokenType::LESS_LESS_THAN;
constexpr JavaTokenType JT_GREAT_GREAT_THAN = JavaTokenType::GREAT_GREAT_THAN;
constexpr JavaTokenType JT_LESS_THAN_EQUALS= JavaTokenType::LESS_THAN_EQUALS;
constexpr JavaTokenType JT_GREAT_THAN_EQUALS= JavaTokenType::GREAT_THAN_EQUALS;
constexpr JavaTokenType JT_ADD_EQUALS= JavaTokenType::ADD_EQUALS;
constexpr JavaTokenType JT_MINUS_EQUALS = JavaTokenType::MINUS_EQUALS;
constexpr JavaTokenType JT_MULT_EQUALS = JavaTokenType::MULT_EQUALS;
constexpr JavaTokenType JT_DIVIDEEQUALS = JavaTokenType::DIVIDEEQUALS;
constexpr JavaTokenType JT_EQUALS = JavaTokenType::EQUALS;
constexpr JavaTokenType JT_PIPEEQUALS= JavaTokenType::PIPEEQUALS;
constexpr JavaTokenType JT_MOD_EQUALS = JavaTokenType::MOD_EQUALS;
constexpr JavaTokenType JT_AMP_EQUALS = JavaTokenType::AMP_EQUALS;
constexpr JavaTokenType JT_XOR_EQUALS = JavaTokenType::XOR_EQUALS;
constexpr JavaTokenType JT_EXCLAM_EQUALS = JavaTokenType::EXCLAM_EQUALS;
constexpr JavaTokenType JT_LESS_LESS_EQUALS = JavaTokenType::LESS_LESS_EQUALS;
constexpr JavaTokenType JT_GREAT_GREAT_EQUALS = JavaTokenType::GREAT_GREAT_EQUALS;
constexpr JavaTokenType JT_OR = JavaTokenType::OR;
constexpr JavaTokenType JT_AND = JavaTokenType::AND;
constexpr JavaTokenType JT_COMMENT = JavaTokenType::COMMENT;
constexpr JavaTokenType JT_BLOCK_COMMENT_OPEN = JavaTokenType::BLOCK_COMMENT_OPEN;
constexpr JavaTokenType JT_BLOCK_COMMENT_CLOSE = JavaTokenType::BLOCK_COMMENT_CLOSE;


constexpr JavaTokenType JT_IDENTIFIER = JavaTokenType::IDENTIFIER;
constexpr JavaTokenType JT_INTEGER = JavaTokenType::INTEGER;
constexpr JavaTokenType JT_DOUBLE = JavaTokenType::DOUBLE;
constexpr JavaTokenType JT_STRING = JavaTokenType::STRING;
constexpr JavaTokenType JT_CHARACTER = JavaTokenType::CHAR;
constexpr JavaTokenType JT_ERROR = JavaTokenType::ERROR;
constexpr JavaTokenType JT_END_OF_FILE = JavaTokenType::END_OF_FILE;

class JavaToken : public Token
{
public:
	static map<string, JavaTokenType> RESERVED_WORDS;
	static map<string, JavaTokenType> SPECIAL_SYMBOLS;
	static map<JavaTokenType, string> SPECIAL_SYMBOL_NAMES;

protected:
	/*
	 * Constructor
	 */

	JavaToken(Source *source) throw (string);

private:
	static bool INITIALIZED;

	/*
	 * Initialize the static maps
	 */

	static void initialize();
};

}}} // namespace wci::frontend::java



#endif /* WCI_FRONTEND_JAVA_JAVATOKEN_H_ */
