/*
 * JavaStringToken.h
 *
 *  Created on: Sep 13, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVASTRINGTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVASTRINGTOKEN_H_

#include <string>
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaStringToken : public JavaToken
{

public:

	/*
	 * constructor
	 * parameter source the source from where to fetch the tokens
	 * throw a string message if an error occured
	 */
	JavaStringToken(Source *source) throw (string);

protected:
	/*
	 * Extract a Java string token from the source.
	 * Override of wci::frontend::Token.
	 * throw a string message if an error occurred
	 */

	void extract() throw(string);
};

}}}} // namespace wci::frontend::java::tokens


#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVASTRINGTOKEN_H_ */
