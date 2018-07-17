/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vars.cpp
 * Author: hsaturn
 * 
 * Created on 17 août 2017, 08:41
 */
#include <functional>

#include "Vars.hpp"


using namespace std;

Vars::Vars() { }

float Vars::getFloat(const string& name, float fdefault) const
{
	auto it = mapVars.find(name);
	if (it != mapVars.end())
		return atof(it->second.c_str());
	return fdefault;
}


long Vars::getLong(const string& name) const
{
	return atol(getString(name).c_str());
}

const string& Vars::getString(const string& name, const string &dflt) const
{
	auto it = mapVars.find(name);
	if (it != mapVars.end())
		return it->second;
	return dflt;
}

void Vars::iterate(function<void(const string&, const string&)> hook)
{
	for(auto it: mapVars)
		hook(it.first, it.second);
}

bool Vars::save(ostream& out) const
{
	out << "vars" << endl << "{" << endl;
	for (auto it : mapVars)
		out << "  " << it.first << '=' << it.second << endl;
	out << '}' << endl;
	return true;
}

bool Vars::load(istream& in)
{
	string s;
	if (!in.good()) return false;
	in >> s;
	if (s == "vars")
	{
		mapVars.clear();
		while (in.good())
		{
			getline(in, s);
			StringUtil::trim(s);
			string::size_type p = s.find('=');
			if (p != string::npos)
			{
				string name = s.substr(0, p);
				StringUtil::trim(name);
				s.erase(0, p + 1);
				StringUtil::trim(s);
				if (name.length())
					mapVars[name] = s;
				else
					cerr << "Empty name (for value " << s << ")" << endl;
			}
			if (s == "}")
				return true;
		}
		return false;
	}
	else
		return false;
}
