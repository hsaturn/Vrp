/* 
 * File:   svr_widget.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#pragma once

#include "cmd.hpp"
#include <Widget.h>

namespace core
{

	class widget : public cmd {
	public:
		widget();
		virtual ~widget(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

