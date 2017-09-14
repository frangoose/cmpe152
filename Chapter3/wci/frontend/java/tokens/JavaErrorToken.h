/*
 * JavaErrorToken.h
 *
 *  Created on: Sep 12, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVAERRORTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVAERRORTOKEN_H_

#include <string>
#include "../JavaError.h"
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaErrorToken : public JavaToken
{

public:
	//Constructor

	JavaErrorToken(Source *source, JavaErrorCode error_code,
			string token_text)
		throw (string);

protected:
	// DO nothing. Do not consume any source characters

	void extract() throw (string);
};

}}}} //namespace wci::frontend::java::tokens

#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVAERRORTOKEN_H_ */
