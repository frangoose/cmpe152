/*
 * JavaCharacterToken.h
 *
 *  Created on: Sep 13, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_

#include <string>
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaCharacterToken : public JavaToken
{
public:
	/*
	 * Constructor
	 * parameter source the source from where to fetch the token
	 * throw a string message if an error occurred
	 */
	JavaCharacterToken(Source *source) throw (string);

protected:
	/*
	 * extract a java character token from the source.
	 * override of wci::frontend::Token.
	 * throw a string message if an error occurred
	 */
	void extract() throw (string);

};

}}}} // namespace wci::frontend::java::tokens

#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_ */
