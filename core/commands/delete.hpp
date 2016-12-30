/* 
 * File:   svr_delete_cmd.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_DELETE_HPP
#define	SVR_DELETE_HPP
#include "cmd.hpp"
#include <map>

namespace core
{

	class delete_cmd : public cmd {
	public:
		delete_cmd();
		virtual ~delete_cmd(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

#endif	/* SVR_DELETE_HPP */

