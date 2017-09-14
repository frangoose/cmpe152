/*
 * JavaNumberToken.h
 *
 *  Created on: Sep 12, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVANUMBERTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVANUMBERTOKEN_H_

#include <string>
#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaNumberToken : public JavaToken
{
public:

	//constructor

	JavaNumberToken(Source *source) throw(string);

protected:
	//extract a java number token from the source
	void extract() throw (string);

	//extract a pascal number token from the source
	void extract_number(string text) throw(string);

private:
	static int MAX_EXPONENT;

	//Extract and return the digits of an unsigned integer
	string unsigned_integer_digits() throw(string);

	//compute and return the integer value of a string of digits
	int compute_integer_value(string digits);

	//compute and return the float value of a real number

	float compute_float_value(string whole_digits, string fraction_digits,
			string exponentDigits, char exponent_sign);
};

}}}} //namespace wci::frontend::pascal::tokens



#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVANUMBERTOKEN_H_ */
