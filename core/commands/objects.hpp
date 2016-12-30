/* 
 * File:   svr_objects.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_OBJECTS_HPP
#define	SVR_OBJECTS_HPP
#include "cmd.hpp"
#include <map>

namespace core
{

	class objects : public cmd {
	public:
		objects();
		virtual ~objects(){};

		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err);

	};

}

#endif	/* SVR_OBJECTS_HPP */

