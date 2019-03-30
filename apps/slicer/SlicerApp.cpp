#include "SlicerApp.hpp"
#include <model/Model.hpp>
#include "fstream"
#include "Printer.hpp"

namespace slicer
{
	SlicerApp::SlicerAppBuilder SlicerApp::builder;
	
	SlicerApp::SlicerApp(const string& name, string& incoming)
	: Application(name), printer(0)
	{
	}
	
	void SlicerApp::init()
	{
		printer = new Printer(this, getString("printer", "default"));
	}
	
	bool SlicerApp::_render(bool resetTimer)
	{
		bool ret = false;
		if (printer)
			ret = printer->_render(resetTimer);
		return ret;	// true if render asap
	}
	
	Application::ExecResult SlicerApp::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue& queue)
	{
		Application::ExecResult result = Application::EXEC_UNKNOWN;
		
		if (printer)
			result = printer->_execute(svr, cmd, incoming, org, queue);
		return result;
	}
	
	void SlicerApp::_help(Help& help)
	{
		help.add("load objfile");
	}
	
	SlicerApp::~SlicerApp()
	{
		string file = builder.getRsrcFileName("setup/slicer.vars");
		ofstream setup(file);
		saveVars(setup);
		
//		saveVars(rsrc);
	}
}