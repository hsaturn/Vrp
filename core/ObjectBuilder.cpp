/* 
 * File:   ObjectBuilder.cpp
 * Author: francois
 * 
 * Created on 20 novembre 2015, 15:29
 */

#include "ObjectBuilder.hpp"
#include "Object.h"
#include "Help.h"
#include "StringUtil.hpp"
#include "Server.h"


Object* ObjectBuilder::buildInstance(const string& sClass, string& incoming)
{
	string orgi(sClass + ' ' + incoming);

	auto it = builders().find(sClass);
	Object* object = 0;
	if (it != builders().end())
	{
		// Comput name
		string base_obj_name = orgi;
		base_obj_name=sClass;
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
		object = it->second->build(obj_name, incoming);
		if (object)
			instances()[obj_name] = object;
	}
	return object;
}

Object* ObjectBuilder::getInstance(const string& name)
{
	for (auto it : instances())
	{
		if (it.second->getName() == name) return it.second;
	}
	return 0;
}

bool ObjectBuilder::execute(Server* server, string cmd, string incoming, const string& org)
{
	bool bRet = false;

	for (auto it : instances())
		bRet = bRet || (it.second->execute(server, cmd, incoming, org));
	return bRet;
}

void ObjectBuilder::renderHud()
{
	for (auto it : instances())
		it.second->renderHud();
}

void ObjectBuilder::render(bool bResetTimer)
{
	for (auto it : instances())
		it.second->render(false);
}

void ObjectBuilder::help(Help& help)
{
	for (auto it : instances())
		it.second->help(help);
}

bool ObjectBuilder::destroyInstance(const string& name)
{
	bool bRet = false;
	auto it=instances().find(name);
	if (it!=instances().end())
	{
		Object* p(it->second);
		instances().erase(it);
		delete p;
		bRet = true;
	}
	return bRet;
}

string ObjectBuilder::listAll()
{
	string list;
	for(auto it: instances())
	{
		list += it.first+' ';
	}
	return list;
}

ObjectBuilder::ObjectBuilder(
const string& sClass)
{
	cout << "         --- NEW CLASS REGISTERED : " << sClass << endl;
	if (sClass.length())
		builders()[sClass] = this;
}

map<string, ObjectBuilder*>& ObjectBuilder::builders()
{
	static map<string, ObjectBuilder*> buildersMap;
	return buildersMap;
}

map<string, Object*>& ObjectBuilder::instances()
{
	static map<string, Object*> instancesMap;
	return instancesMap;
}
	
