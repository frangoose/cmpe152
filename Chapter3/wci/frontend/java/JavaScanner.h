/*
 * JavaScanner.h
 *
 *  Created on: Sep 11, 2017
 *      Author: Nick
 */

#ifndef WCI_FRONTEND_JAVA_JAVASCANNER_H_
#define WCI_FRONTEND_JAVA_JAVASCANNER_H_

#include "../Scanner.h"
#include "../Source.h"
#include "../Token.h"

namespace wci { namespace frontend { namespace java {

using namespace std;
using namespace wci::frontend;

class JavaScanner : public Scanner
{
public:
	//Constructor
	JavaScanner(Source *source);

protected:
	Token *extract_token() throw (string);

private:
	/*
	 * need to skip whitespace characters and
	 * comments
	 */

	void skip_white_space() throw (string);
};

}}} // namespace wci::frontend::java



#endif /* WCI_FRONTEND_JAVA_JAVASCANNER_H_ */
