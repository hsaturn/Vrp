/* 
 * File:   svr_match.cpp
 * Author: francois
 * 
 */

#include "match.hpp"
#include <ansi_colors.hpp>

using namespace std;

namespace core {
	match match_instance;

	match::match() {
		registerSyntax("match", "pattern string", "Test of StringUtil::preg_match");
	}

	bool match::run(Server* server, string &sCmd, string& sArgs, stringstream& out, stringstream& err) {
		bool bRet = false;

		string s1 = StringUtil::getWord(sArgs);
		string s2 = StringUtil::getWord(sArgs);
		cout << Ansi::cyan() << "Match " << s1 << " / " << s2 << endl;
		if (StringUtil::preg_match(s1, s2, false))
			cout << Ansi::green() << "OK" << endl;
		else
			cout << Ansi::red() << "KO" << endl;
		cout << Ansi::reset();
		return bRet;
	}
}