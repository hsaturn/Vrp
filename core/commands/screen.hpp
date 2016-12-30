/* 
 * File:   svr_screen.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_SCREEN_HPP
#define	SVR_SCREEN_HPP
#include "cmd.hpp"
#include <map>

namespace core
{

	class screen : public cmd {
	public:
		screen();
		virtual ~screen(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

#endif	/* SVR_SCREEN_HPP */

