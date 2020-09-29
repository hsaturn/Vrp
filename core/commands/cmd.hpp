/* 
 * File:   cmd.hpp
 * Author: francois
 *
 * Created on 4 août 2014, 14:47
 */

#ifndef CMD_HPP
#define	CMD_HPP
 
#include "../Server.h"
#include <StringUtil.hpp>
#include "Commands.hpp"
#include <String.hpp>
#include "ansi_colors.hpp"

// TODO à déplacer dans la nouvelle classe SearchServer
extern unsigned long giMaxThreads;
extern unsigned long giMaxThreadsReached;
extern float gfUseViewCounter;

namespace core
{
	// oCommands.addCommand("stats"	, ""												, "Affiche quelques stats");
	
	class cmd
	{
	public:
		
		cmd();
		virtual ~cmd();
		
		// Many syntaxes, but at least one can be registered for one keyword (name)
		void registerSyntax(const char* name, const char* args="", const char* desc="");
		
		static bool execute(String sCmd, String sArgs, Server *server);
		
		static bool executeFile( string &sFile, Server* request=0);
		
		static Commands* getCommands();
		
	protected:
		virtual bool run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)=0;
		
		static map<string, cmd*>*	mapCmd;
		
		Server* psvr;
		
	private:
		static Commands* commands;
	};

}

#endif	/* CMD_HPP */

