/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   slicer.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 07:11
 */

#pragma once

#include <ObjectBuilder.hpp>
#include <string>
#include <vector>

#include "SlicerModels.hpp"

using namespace std;

namespace slicer
{
	class Printer;

	class SlicerApp : public Application
	{
		APPLICATION_BUILDER("slicer", SlicerApp);
		
	  public:
		SlicerApp(const string& name, string &incoming);
		void init() override;
		~SlicerApp() override;

		bool _render(bool resetTimer) override;
		Application::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) override;
		void best(Server*);
		void pop();

	  private:

		bool slice();

		SlicerApp(const SlicerApp& orig);

		Printer* printer = nullptr;
	};
}


