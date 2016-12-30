/* 
 * File:   cmd.cpp
 * Author: francois
 * 
 * Created on 4 août 2014, 14:47
 */

#include "cmd.hpp"
#include "ansi_colors.hpp"
#include <../Server.h>
#include <iostream>
#include <fstream>
#include <string>

namespace core
{
  Commands* cmd::commands = 0;
  map<string, cmd*>* cmd::mapCmd = 0;
  //SocketServer* cmd::mrequest = 0;

  cmd::cmd() { }

  cmd::~cmd() { }

  Commands* cmd::getCommands() {
	  if (commands==0)
		  commands = new Commands;
	  return commands;
  }

  void cmd::registerSyntax(const char* name, const char* args, const char* desc)
  {
	  if (mapCmd == 0)
	  {
		  mapCmd = new map<string, cmd*>;
	  }
	  Commands* pCommands = getCommands();
	  pCommands->addCommand(name, args, desc);

	  (*mapCmd)[name] = this;
  }

  bool cmd::execute(String sCmd, String sArgs, Server *psvr)
  {
	  bool bRet(false);
	  stringstream output;
	  stringstream err;

	  map<string, cmd*>::iterator it = mapCmd->find(sCmd);

	  if (it != mapCmd->end())
	  {
		  it->second->psvr = psvr;
		  bRet = it->second->run(psvr, sCmd, sArgs, output, err);
	  }
	  else
		  cerr << Ansi::red() << "Unknown command : " << sCmd << Ansi::reset() << endl;
	  if (psvr)
	  {
		  psvr->send(output.str());
		  if (err.str().length())
			  psvr->send(string(Ansi::red())+err.str()+Ansi::reset()+"\n");
	  }
	  else
	  {
		  cout << output.str();
		  if (err.str().length())
		  {
			  cerr << Ansi::red() << err.str() << ' ' << Ansi::reset();
			  bRet = false;
		  }
	  }

	  return bRet;
  }

  bool cmd::executeFile(string &sFile, Server* psvr)
  {
	  bool bRet = true;
	  cout << "Running configuration file " << sFile << endl;
	  cout << "--------------------------" << endl;
	  ifstream o(sFile.c_str());
	  string sLine;

	  while (getline(o, sLine))
	  {
		  bool bEcho(true);
		  string sArgs(sLine);
		  string sCmd = StringUtil::getWord(sArgs);

		  if (sCmd[0] != '@')
		  {
			  cout << "  * " << sCmd << ' ' << sArgs << ' ';
		  }
		  else
		  {
			  sCmd.erase(0, 1);
			  bEcho = false;
		  }

		  bool bExec = execute(sCmd, sArgs, psvr);
		  if (bEcho)
		  {
			  if (bExec)
			  {
				  cout << Ansi::green() << " ... OK ";
			  }
			  else
			  {
				  cout << Ansi::red() << " ... KO ";
			  }
		  }
		  cout << Ansi::reset() << endl;
	  }
	  cout << "--------------------------" << endl;
	  return bRet;
  }
}