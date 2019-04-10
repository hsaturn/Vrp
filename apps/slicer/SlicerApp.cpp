#include "SlicerApp.hpp"
#include <model/Model.hpp>
#include "fstream"
#include "Printer.hpp"
#include "AutoDetect.hpp"

namespace slicer
{
	SlicerApp::SlicerAppBuilder SlicerApp::builder;

	SlicerApp::SlicerApp(const string& name, string& incoming)
	: Application(name)
	{
      AutoDetect printers;
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

	IRunnable::ExecResult SlicerApp::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue& queue)
	{
		IRunnable::ExecResult result = IRunnable::EXEC_UNKNOWN;

      string sName(cmd);
      sName = getWord(sName, ".");
      if (printer && sName == printer->name())
      {
         getWord(cmd, ".");
			result = printer->_execute(svr, cmd, incoming, org, queue);
      }

      if (result == IRunnable::EXEC_UNKNOWN)
         result = printer->_execute(svr, cmd, incoming, org, queue);

		return result;
	}

	void SlicerApp::_help(Help& help) const
	{
      if (printer)
      {
         auto pc=help.pushClass(printer->name());
         printer->_help(help);
      }
	}

	SlicerApp::~SlicerApp()
	{
		string file = builder.getRsrcFileName("setup/slicer.vars");
		ofstream setup(file);
		saveVars(setup);

//		saveVars(rsrc);
	}
}