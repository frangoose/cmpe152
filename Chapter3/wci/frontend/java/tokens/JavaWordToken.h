/*
 * JavaWordToken.h
 *
 *  Created on: Sep 12, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVAWORDTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVAWORDTOKEN_H_

#include <string>
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaWordToken : public JavaToken
{
public:
	/*
	 * Constructor
	 * source the soruce from where to fetch the token's characters.
	 * throw a string message if an error occured
	 */

	JavaWordToken(Source *source) throw (string);

protected:
	/*
	 * Extract a java word token from the source
	 * override of wci::frontend::Token.
	 * throw a string message if an error occurred
	 */
	void extract() throw (string);
};

}}}} // namespace wci::frontend::pascal::tokens

#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVAWORDTOKEN_H_ */
