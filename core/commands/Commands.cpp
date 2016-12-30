#include <Commands.hpp>
#include <iomanip>
#include <iostream>
#include <StringUtil.hpp>
#include <String.hpp>


using namespace std;

Commands::~Commands()
{
	for(iterator it=begin(); it != end(); it++)
	{
		delete it->second;
	}
}

void Command::serialize(ostream& out) const
{
	out << "[0";
	out << StringUtil::serialize(msCmd);
	out << StringUtil::serialize(msArgs);
	out << StringUtil::serialize(msDesc);
	out << "]";
 }

Command* Command::unserialize(string &s) 
{
	Command* p;
	StringUtil::trim(s,true);
	if (s.substr(0,2)!="[0")
	{
		throw string("Command : Unable to unserialize Command, got (")+s.substr(0,2)+") instead of [x";
	}
	s.erase(0,2);
	p=new Command;
	p->msCmd=StringUtil::unserialize(s);
	p->msArgs=StringUtil::unserialize(s);
	p->msDesc=StringUtil::unserialize(s);
	if (s[0]!=']')
		throw string("Command ']' expected at end of serialized Command");
	s.erase(0,1);
	return p;
}

void Commands::addCommand(const string& sCmd, const string& sArgs, const string& sDescription)
{
	mapCommands.insert(pair<string, Command*>(sCmd,new Command(sCmd, sArgs, sDescription)));
}

void Commands::addCommand(Command *p)
{
	mapCommands.insert(pair<string, Command*>(p->getCmd(),p));
}

void Commands::show(ostream& out, const string& sIndent, string sWhat)
{
	string::size_type iMaxCmdArgs=0;

	for(const_iterator oit=begin(); oit!=end(); oit++)
	{
		String sCmd(oit->second->getCmd());
		
		if (oit->second->getDesc().length() && sCmd.find(sWhat) != string::npos)
		{
			iMaxCmdArgs=std::max(iMaxCmdArgs, (oit->second->cmdArgs()).length());
		}
	}
	iMaxCmdArgs++;

	for(const_iterator oit=begin(); oit!=end(); oit++)
	{
		String sCmd(oit->second->getCmd());
		
		// cout << "CMD(" << sCmd << ") " << sWhat << ( sCmd.startsWith(sWhat) ? " OK " : " -- ") << oit->second->getDesc().length()<< endl;
		
		if (oit->second->getDesc().length() && sCmd.find(sWhat) != string::npos)
		{
			out << sIndent << setw(iMaxCmdArgs) << setiosflags(ios::left) << oit->second->cmdArgs() << " " << oit->second->getDesc() << endl; 
		}
	}
}

void Commands::serialize(ostream& out)
{
	out << '{' ;
	for(const_iterator oit=begin(); oit!=end(); oit++)
	{
		oit->second->serialize(out);
		out << endl;
	}
	out << '}';
}

void Commands::unserialize(string& s)
{
	StringUtil::trim(s);
	if (s[0]!='{')
	{
		throw string("Command '{' expected for serialized Commands");
	}
	s.erase(0,1); // Remove {
	do
	{
		StringUtil::trim(s);
		addCommand(Command::unserialize(s));
		StringUtil::trim(s,true);
		if (s[0]=='}')
		{
			break;
		}
		else if (s[0]!='[')
			s.erase(0,1);
	}
	while(s.length());
}

bool Commands::makeArgsCompletion(string sInput, string sCmdStart, string& sArgs, Command* cmd, TContainer& results, char end)
{
	if (sCmdStart[sCmdStart.length()-1] != ' ')	sCmdStart += ' ';
	
	while(sArgs.length())
	{
		if (sArgs[0]==end) return false;
		if (sArgs[0]=='|')
		{
			sArgs.erase(0,1);
		}
		else if (sArgs[0]=='[')	// option
		{
			sArgs.erase(0,1);
			if (sInput.length()<sCmdStart.length())
				results.insert(pair<string,Command*>(sCmdStart,cmd));
			
			if (!makeArgsCompletion(sInput, sCmdStart, sArgs, cmd, results, ']'))
				return false;
		}
		else if (sArgs[0]=='(')	// choix
		{
			makeArgsCompletion(sInput, sCmdStart, sArgs, cmd, results,')');
		}
		else if (isdigit(sArgs[0]))
		{
			results.insert(pair<string,Command*>(sCmdStart,cmd));
			StringUtil::getLong(sArgs);
		}
		else
		{
			try
			{
				string id=StringUtil::getIdentifier(sArgs);
				if (id=="str" || id=="file")
				{
					// special
				}
				else
				{
					String sComp = sCmdStart+id;
					if (sComp.startsWith(sInput))
						results.insert(pair<string,Command*>(sComp,cmd));
				}
			}
			catch(string e)
			{
				break;
			}
		}
	}
	return true;
}

void Commands::getCompletion(const string& sInput, TContainer& results)
{
	for(auto oit=begin(); oit!=end(); oit++)
	{
		String sCmd = oit->second->getCmd();
		string sArgs = oit->second->getArgs();
		if (sCmd.startsWith(sInput))
		{
			if (sCmd.length()>=sInput.length())
			{
				results.insert(pair<string,Command*>(sCmd,oit->second));
			}
			else
			{
				makeArgsCompletion(sInput, sCmd, sArgs, oit->second, results);
			}
		}
	}
}

