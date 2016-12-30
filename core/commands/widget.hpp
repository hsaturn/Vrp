/* 
 * File:   svr_widget.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_WIDGET_CMD_HPP
#define	SVR_WIDGET_CMD_HPP
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

#endif	/* SVR_WIDGET_CMD_HPP */

