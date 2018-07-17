/* 
 * File:   svr_match.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_MATCH_HPP
#define	SVR_MATCH_HPP
#include "cmd.hpp"

namespace core
{

	class match : public cmd {
	public:
		match();
		virtual ~match(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);
	};

}

#endif	/* SVR_MATCH_HPP */

