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

#ifndef SLICER_HPP
#    define SLICER_HPP

#    include <Object.h>
#    include <ObjectBuilder.hpp>
#    include <string>
#    include <vector>

#    include "SlicerModels.hpp"

using namespace std;

namespace slicer
{
	class Printer;

	class Slicer : public Application
	{
		APPLICATION_BUILDER("slicer", Slicer);
		
	  public:
		Slicer(const string& name, string &incoming);
		void init() override;
		~Slicer() override;

		bool _render(bool resetTimer) override;
		Application::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) override;
		void best(Server*);
		void pop();

	  private:

		bool slice();

		static SlicerBuilder appclass;

		Slicer(const Slicer& orig);

		Printer* printer;

	};
}


#endif /* SLICER_HPP */

