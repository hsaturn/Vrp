/* 
 * File:   svr_test.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_TEST_HPP
#define	SVR_TEST_HPP
#include "cmd.hpp"
#include <map>

namespace core
{

	class test : public cmd {
	public:
		test();
		virtual ~test(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

#endif	/* SVR_TEST_HPP */

