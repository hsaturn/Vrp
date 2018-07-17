/* 
 * File:   svr_new_cmd.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_NEW_HPP
#define	SVR_NEW_HPP
#include "cmd.hpp"
#include <map>

namespace core
{

	class new_cmd : public cmd {
	public:
		new_cmd();
		virtual ~new_cmd(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);
	};

}

#endif	/* SVR_NEW_HPP */

