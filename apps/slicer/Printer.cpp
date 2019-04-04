/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Printer.cpp
 * Author: hsaturn
 * 
 * Created on 15 août 2017, 08:23
 */

#include "Printer.hpp"
#include "Object.h"
#include "SlicerApp.hpp"

namespace slicer
{

	Printer::Printer(SlicerApp* parent, string config)
	{
		cerr << "NEW PRINTER " << config << endl;
		string file = parent->getRsrcFileName("printers/"+config);
	}

	Printer::~Printer() { }

	Application::ExecResult Printer::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue& queue)
	{
		Application::ExecResult result = Application::EXEC_UNKNOWN;
		if (cmd == "load")
			result = models.load(incoming);

		if (result == Application::EXEC_UNKNOWN)
		{
			result = models.execute(svr, cmd, incoming, org, queue);
		}

		return result;
	}

	void Printer::_help(Help&) { }

	bool Printer::_render(bool resetTimer)
	{
		models.render(resetTimer);
		return false;
	}

}
