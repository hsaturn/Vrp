/* 
 * File:   ObjectBuilder.hpp
 * Author: francois
 *
 * Created on 20 novembre 2015, 15:29
 */

#ifndef OBJECTBUILDER_HPP
#define	OBJECTBUILDER_HPP

#include "Help.h"
#include <string>
#include <list>
#include "Object.h"

using namespace std;

class Server;

class ApplicationBuilder {
public:
	virtual Application* build(const string& name, string& incoming) = 0;
	const string getAppClass() const;
	virtual ~ApplicationBuilder(){};

	static Application* buildInstance(const string& sClass, string&);

	static bool destroyInstance(const string& name);
   
   static bool destroyAll();
   
	static Application* getInstance(const string& name);

	static Application::ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);

	static bool render(bool resetTimer);
	
	static void renderHud();

	static void help(Help& help);

	static string listAll();
	
	string getRsrcFileName(const string rel) const;

protected:
	ApplicationBuilder(const string& app_class);

private:
	ApplicationBuilder(const ApplicationBuilder& orig);

	static map<string, ApplicationBuilder*>& builders();
	static map<string, Application*>& instances();
	
	string appclass;
	
};

#endif	/* OBJECTBUILDER_HPP */

