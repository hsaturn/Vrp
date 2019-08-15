/* 
 * File:   ObjectBuilder.cpp
 * Author: francois
 * 
 * Created on 20 novembre 2015, 15:29
 */

#include "ObjectBuilder.hpp"
#include "Help.h"
#include "StringUtil.hpp"
#include "Server.h"
#include <sys/stat.h>
#include <set>

Application* ApplicationBuilder::buildInstance(const string& sClass, string& incoming)
{
	string orgi(sClass + ' ' + incoming);

	auto it = builders().find(sClass);
	Application* application = 0;
	if (it != builders().end())
	{
		// Comput name
		string base_obj_name = orgi;
		base_obj_name = sClass;
		StringUtil::trim(base_obj_name);
		while (base_obj_name.find(' ') != string::npos) base_obj_name[base_obj_name.find(' ')] = '_';
		int counter = 1;
		string obj_name = base_obj_name;
		while (instances().find(obj_name) != instances().end())
		{
			obj_name = base_obj_name + '_' + StringUtil::to_string(counter);
			counter++;
		}

		// Enregistrer l'objet dans une map
		application = it->second->build(obj_name, incoming);
		if (application)
		{
			instances()[obj_name] = application;
			application->builder = it->second;
			application->init();
		}
	}
	return application;
}

const string ApplicationBuilder::getAppClass() const
{
	return appclass;
}

Application* ApplicationBuilder::getInstance(const string& name)
{
	for (auto it : instances())
	{
		if (it.second->getName() == name) return it.second;
	}
	return 0;
}

Application::ExecResult ApplicationBuilder::execute(Server* server, string cmd, string incoming, const string& org, CmdQueue& queue)
{
	Application::ExecResult ret;

	for (auto it : instances())
	{
		ret = it.second->execute(server, cmd, incoming, org, queue);
		if (ret == Application::EXEC_UNKNOWN)
			continue;
		else if (ret == Application::EXEC_BUSY)
		{
			queue.push_front(org);
			return Application::EXEC_BUSY;
		}
		else
			return ret;
	}
	return Application::EXEC_UNKNOWN;
}

void ApplicationBuilder::renderHud()
{
	for (auto it : instances())
		it.second->renderHud();
}

bool ApplicationBuilder::render(bool bResetTimer)
{
	bool bRet = false;
	for (auto it : instances())
		bRet |= it.second->render(false);
	return bRet;
}

void ApplicationBuilder::help(Help& help)
{
	for (const auto& it : instances())
		it.second->help(help);
}

bool ApplicationBuilder::destroyAll()
{
   bool bRet = true;
   set<string> names;
   for(const auto& [name, instance] : instances())
   {
      names.insert(name);
   }
   
   for(const auto& name: names)
   {
      bRet &= destroyInstance(name);
   }
   return bRet;
}

bool ApplicationBuilder::destroyInstance(const string& name)
{
	bool bRet = false;
	auto it = instances().find(name);
	if (it != instances().end())
	{
		Application * p(it->second);
		instances().erase(it);
		delete p;
		bRet = true;
	}
	return bRet;
}

string ApplicationBuilder::listAll()
{
	string list;
	for (auto it : instances())
	{
		list += it.first + ' ';
	}
	return list;
}

ApplicationBuilder::ApplicationBuilder(const string& sClass)
{
	appclass = sClass;
	if (builders().find(sClass) != builders().end())
		throw "Application class " + sClass + " already registered.";

	appclass = sClass;
	cout << "         --- NEW APPLICATION CLASS REGISTERED : " << sClass << endl;
	if (sClass.length())
		builders()[sClass] = this;
}

string ApplicationBuilder::getRsrcFileName(const string rel) const
{
	string filename = "data/apps/" + appclass + "/" + rel;

	string::size_type sl = filename.find('/');
	while (sl != string::npos)
	{
		mkdir(filename.substr(0, sl).c_str(), 0766);
		sl = filename.find('/', sl + 1);
	}
	return filename;
}

map<string, ApplicationBuilder*>& ApplicationBuilder::builders()
{
	static map<string, ApplicationBuilder*> buildersMap;
	return buildersMap;
}

map<string, Application*>& ApplicationBuilder::instances()
{
	static map<string, Application*> instancesMap;
	return instancesMap;
}

