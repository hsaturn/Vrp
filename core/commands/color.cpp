/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   color.cpp
 * Author: hsaturn
 * 
 * Created on 19 août 2017, 09:35
 */

#include "color.hpp"
#include "renderer/opengl/Color.h"

namespace core
{
	static color_cmd color_instance;

	color_cmd::color_cmd()
	{
		registerSyntax("color", "[name=][#rvb[a]|#rrvvbb[aa]][r v b]", "Color declaration");
	}

	color_cmd::~color_cmd() { }

	bool color_cmd::run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
	{
		bool bRet = true;

		const Color* c = Color::factory(sArgs);
		
		if (c==0)
		{
			bRet = false;
			cout << Ansi::red() << " bad color " << sArgs << endl;
		}

		return bRet;
	}
}

