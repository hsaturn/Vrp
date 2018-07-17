/* 
 * File:   svr_algo.hpp
 * Author: francois
 *
 * Created on 6 août 2014, 10:42
 */

#ifndef SVR_TEST_HPP
#define	SVR_TEST_HPP
#include <core/commands/cmd.hpp>

namespace core
{

	class algo : public cmd {
	public:
		algo();
		~algo() override {};

		bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err) override;

	};

}

#endif	/* SVR_TEST_HPP */

