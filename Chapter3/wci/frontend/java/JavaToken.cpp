/*
 * JavaToken.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: Nick
 */
#include <string>
#include <vector>
#include <map>
#include "JavaToken.h"

namespace wci { namespace frontend { namespace java {

using namespace std;

map<string, JavaTokenType> JavaToken::RESERVED_WORDS;
map<string, JavaTokenType> JavaToken::SPECIAL_SYMBOLS;
map<JavaTokenType, string> JavaToken::SPECIAL_SYMBOL_NAMES;

bool JavaToken::INITIALIZED = false;

void JavaToken::initialize()
{
	if (INITIALIZED) return;

	vector<string> rw_strings =
	{
			"ABSTRACT", "DOUBLE", "INT", "LONG", "BREAK", "ELSE", "LONG",
			"SWITCH", "CASE", "ENUM", "NATIVE", "SUPER", "CHAR", "EXTENDS",
			"RETURN", "THIS", "CLASS", "FLOAT", "SHORT", "THROW", "CONST",
			"FOR", "PACKAGE", "VOID", "CONTINUE", "GOTO", "PROTECTED", "VOLATILE",
			"DO", "IF", "STATIC", "WHILE"
	};

	vector<JavaTokenType> rw_keys =
	{
		JavaTokenType::ABSTRACT,
		JavaTokenType::DOUBLE,
		JavaTokenType::INT,
		JavaTokenType::LONG,
		JavaTokenType::BREAK,
		JavaTokenType::ELSE,
		JavaTokenType::LONG,
		JavaTokenType::SWITCH,
		JavaTokenType::CASE,
		JavaTokenType::ENUM,
		JavaTokenType::NATIVE,
		JavaTokenType::SUPER,
		JavaTokenType::CHAR,
		JavaTokenType::EXTENDS,
		JavaTokenType::RETURN,
		JavaTokenType::THIS,
		JavaTokenType::CLASS,
		JavaTokenType::FLOAT,
		JavaTokenType::SHORT,
		JavaTokenType::THROW,
		JavaTokenType::CONST,
		JavaTokenType::FOR,
		JavaTokenType::PACKAGE,
		JavaTokenType::VOID,
		JavaTokenType::CONTINUE,
		JavaTokenType::GOTO,
		JavaTokenType::PROTECTED,
		JavaTokenType::VOLATILE,
		JavaTokenType::DO,
		JavaTokenType::IF,
		JavaTokenType::STATIC,
		JavaTokenType::WHILE

	};

    for (int i = 0; i < rw_strings.size(); i++)
    {
        RESERVED_WORDS[rw_strings[i]] = rw_keys[i];
    }

    vector<string> ss_strings =
    {
    	"~", "!", "@", "%", "^", "&", "*", "-", "+", "=", "|", "/",
		":", ";", "?", "<", ">", ".", ",", "\'", "\"", "(", ")", "[",
		"]", "{", "}", "++", "--", "<<", ">>", "<=", ">=", "+=",
		"-=", "*=", "/=", "==", "|=", "%=", "&=", "^=", "!=", "<<=",
		">>=", "||", "&&", "//", "/*", "*/"
    };

    vector<JavaTokenType> ss_keys =
    {
    		JavaTokenType::TILDE,
			JavaTokenType::EXCLAMATION,
			JavaTokenType::AT,
			JavaTokenType::MOD,
			JavaTokenType::XOR,
			JavaTokenType::AMPERSAND,
			JavaTokenType::MULTIPLY,
			JavaTokenType::MINUS,
			JavaTokenType::ADD,
			JavaTokenType::ASSIGN,
			JavaTokenType::PIPE,
			JavaTokenType::DIVIDE,
			JavaTokenType::COLON,
			JavaTokenType::SEMICOLON,
			JavaTokenType::QUESTION,
			JavaTokenType::LESS_THAN,
			JavaTokenType::GREATER_THAN,
			JavaTokenType::DOT,
			JavaTokenType::COMMA,
			JavaTokenType::SINGLE_QUOTE,
			JavaTokenType::DOUBLE_QUOTE,
			JavaTokenType::OPEN_PAR,
			JavaTokenType::CLOSE_PAR,
			JavaTokenType::OPEN_BRACK,
			JavaTokenType::CLOSE_BRACK,
			JavaTokenType::OPEN_CURLY,
			JavaTokenType::CLOSE_CURLY,
			JavaTokenType::INCREMENT,
			JavaTokenType::DECREMENT,
			JavaTokenType::LESS_LESS_THAN,
			JavaTokenType::GREAT_GREAT_THAN,
			JavaTokenType::LESS_THAN_EQUALS,
			JavaTokenType::GREAT_THAN_EQUALS,
			JavaTokenType::ADD_EQUALS,
			JavaTokenType::MINUS_EQUALS,
			JavaTokenType::MULT_EQUALS,
			JavaTokenType::DIVIDEEQUALS,
			JavaTokenType::EQUALS,
			JavaTokenType::PIPEEQUALS,
			JavaTokenType::MOD_EQUALS,
			JavaTokenType::AMP_EQUALS,
			JavaTokenType::XOR_EQUALS,
			JavaTokenType::EXCLAM_EQUALS,
			JavaTokenType::LESS_LESS_EQUALS,
			JavaTokenType::GREAT_GREAT_EQUALS,
			JavaTokenType::OR,
			JavaTokenType::AND,
			JavaTokenType::COMMENT,
			JavaTokenType::BLOCK_COMMENT_OPEN,
			JavaTokenType::BLOCK_COMMENT_CLOSE
    };

    for (int i = 0; i < ss_strings.size(); i++){
    	SPECIAL_SYMBOLS[ss_strings[i]] = ss_keys[i];
    }

    vector<string> ss_names =
    {
    	    "TILDE", "EXCLAMATION", "AT", "MOD", "XOR", "AMPERSAND", "MULTIPLY",
    		"MINUS", "ADD", "ASSIGN", "PIPE", "DIVIDE", "COLON", "SEMICOLON",
    		"QUESTION", "LESS_THAN", "GREATER_THAN", "DOT", "COMMA", "SINGLE_QUOTE",
    		"DOUBLE_QUOTE", "OPEN_PAR", "CLOSE_PAR", "OPEN_BRACK", "CLOSE_BRACK", "OPEN_CURLY",
    		"CLOSE_CURLY", "INCREMENT", "DECREMENT", "LESS_LESS_THAN", "GREAT_GREAT_THAN",
    		"LESS_THAN_EQUALS", "GREAT_THAN_EQUALS", "ADD_EQUALS", "MINUS_EQUALS",
    		"MULT_EQUALS", "DIVIDEEQUALS", "EQUALS", "PIPEEQUALS", "MOD_EQUALS",
    		"AMP_EQUALS", "XOR_EQUALS", "EXCLAM_EQUALS", "LESS_LESS_EQUALS",
    		"GREAT_GREAT_EQUALS", "OR", "AND", "COMMENT", "BLOCK_COMMENT_OPEN", "BLOCK_COMMENT_CLOSE"

    };

    for (int i = 0; i < ss_names.size(); i++){
    	SPECIAL_SYMBOL_NAMES[ss_keys[i]] = ss_names[i];
    }

    INITIALIZED = true;
}

JavaToken::JavaToken(Source *source) throw (string)
		: Token(source)
{
	initialize();
}

}}} //namespace wci::frontend::java
