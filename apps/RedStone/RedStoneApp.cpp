/* 
 * File:   RedStone.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 20:36
 */

#include "RedStoneApp.hpp"

namespace RedStone
{
	RedStoneApp::RedStoneAppBuilder RedStoneApp::builder;
	
	RedStoneApp::RedStoneApp(const string& name, string& incoming)
	: Application(name)
	{
		
	}
	
	IRunnable::ExecResult RedStoneApp::_execute(Server*, string cmd, string incoming, const string& org, CmdQueue&)
	{
		return EXEC_UNKNOWN;
	}
	
	void RedStoneApp::_help(Help&)
	{
		
	}
	
	bool RedStoneApp::_render(bool resetTimer)
	{
		return false;
	}

}
