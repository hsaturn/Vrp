/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   color.hpp
 * Author: hsaturn
 *
 * Created on 19 août 2017, 09:35
 */

#ifndef COLOR_HPP
#    define COLOR_HPP

#include "cmd.hpp"

namespace core
{

	class color_cmd : public cmd
	{
	  public:
		color_cmd();
		color_cmd(const color_cmd& orig);
		~color_cmd() override;
		bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err) override;
	};
}
#endif /* COLOR_HPP */

