/* 
 * File:   svr_widget.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#pragma once

#include "cmd.hpp"

namespace core
{

	class font : public cmd {
	public:
		font();
		virtual ~font(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

