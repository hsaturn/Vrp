#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <map>

using namespace std;

class Command
{
	private:
		string msCmd;
		string msArgs;
		string msDesc;

	public:
		Command(){};
		
		Command(const string &sCmd, const string &sArgs, const string &sDesc)
			: msCmd(sCmd), msArgs(sArgs), msDesc(sDesc) {};

		const string& getCmd()		const { return msCmd; }
		const string& getArgs()	const { return msArgs; }
		const string& getDesc()	const { return msDesc; }
        const string cmdArgs()		const { return msCmd+" "+msArgs; }

		bool matchBegin(const string& sMatch) const { return msCmd.substr(0,sMatch.length())==sMatch; }

		void serialize(ostream& out) const;
        static Command* unserialize(string &s);
};

class Commands
{
	public:
		typedef multimap<string, Command*>		TContainer;
		typedef TContainer::iterator		iterator;
		typedef TContainer::const_iterator	const_iterator;

	private:
		TContainer mapCommands;

	public:
		Commands() {};
		~Commands();
        
		void addCommand(const string& sCmd, const string& sArgs="", const string& sDescription="");
        void addCommand(Command*);
        
		/**
		  * Display commands, only those having a non empty description
		  * @param ostream& out, where to output commands
		  * @param string sIndent, indentation
		  * @param string sWhat, restrict to sWhat only
		  * @return void
		  **/
		void show(ostream& out, const string& sIndent, string sWhat="");
        
		void serialize(ostream& out);
        void unserialize(string &s);
        
		void getCompletion(const string& sCmd, TContainer &result);
		bool exists(const string& s) const { return mapCommands.find(s)!=mapCommands.end();}

		void clear() { mapCommands.clear(); }

		iterator		begin()			{ return mapCommands.begin();	};
		iterator		end()			{ return mapCommands.end();		};
        iterator		find(const string& s)
                                        { return mapCommands.find(s);	};

		const_iterator	begin() const	{ return mapCommands.begin();	};
		const_iterator	end()	const	{ return mapCommands.end();		};
        const_iterator	find(const string&s) const
										{ return mapCommands.find(s);	};
        
        bool haveCommand(const string& sCmd) const { return find(sCmd)!=end(); }

	private:
		Commands(const Commands&){};
		Commands& operator=(const Commands&);
		
		bool makeArgsCompletion(string sInput, string sCmdStart, string& sArgs, Command* cmd, TContainer& results, char end=0);
};

#endif
