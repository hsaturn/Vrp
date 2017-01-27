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

using namespace std;

class Object;
class Server;

class ObjectBuilder {
public:
	virtual Object* build(const string& name, string& incoming) = 0;

	static Object* buildInstance(const string& sClass, string&);

	static bool destroyInstance(const string& name);

	static Object* getInstance(const string& name);

	static bool execute(Server*, string cmd, string incoming, const string& org);

	static bool render(bool resetTimer);
	
	static void renderHud();

	static void help(Help& help);

	static string listAll();

protected:
	ObjectBuilder(const string& name);

private:
	ObjectBuilder(const ObjectBuilder& orig);

	static map<string, ObjectBuilder*>& builders();
	static map<string, Object*>& instances();
	
};

#endif	/* OBJECTBUILDER_HPP */

