/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Printer.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 08:23
 */

#ifndef PRINTER_HPP
#    define PRINTER_HPP

#    include "ostream"
#    include <core/Object.h>
#    include "SlicerModels.hpp"

using namespace std;

namespace slicer
{

	class Slicer;

	class Printer : public Renderable
	{
	  public:
		Printer(Slicer* parent, string config);
		~Printer() override;

		Application::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&)			  override;
		bool _render(bool resetTimer) override;
		
		bool load(string);
	  private:
		Printer(const Printer& orig);

	  private:
		SlicerModels models;

	};

}

#endif /* PRINTER_HPP */

