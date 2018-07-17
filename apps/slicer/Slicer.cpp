#include "Slicer.hpp"
#include <model/Model.hpp>
#include "fstream"
#include "Printer.hpp"

namespace slicer
{
	Slicer::SlicerBuilder Slicer::appclass;
	
	Slicer::Slicer(const string& name, string& incoming)
	: Application(name), printer(0)
	{
	}
	
	void Slicer::init()
	{
		printer = new Printer(this, getString("printer", "default"));
	}
	
	bool Slicer::_render(bool resetTimer)
	{
		bool ret = false;
		if (printer)
			ret = printer->_render(resetTimer);
		return ret;	// true if render asap
	}
	
	Application::ExecResult Slicer::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue& queue)
	{
		Application::ExecResult result = Application::EXEC_UNKNOWN;
		
		if (printer)
			result = printer->_execute(svr, cmd, incoming, org, queue);
		return result;
	}
	
	void Slicer::_help(Help& help)
	{
		help.add("load objfile");
	}
	
	Slicer::~Slicer()
	{
		string file = appclass.getRsrcFileName("setup/slicer.vars");
		ofstream setup(file);
		saveVars(setup);
		
//		saveVars(rsrc);
	}
}