/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vars.hpp
 * Author: hsaturn
 *
 * Created on 17 août 2017, 08:41
 */

#ifndef VARS_HPP
#    define VARS_HPP
#    include <iostream>
#    include <map>

#    include "StringUtil.hpp"
#include <functional>

using namespace std;

class Vars
{
  public:
	Vars();

	virtual ~Vars() { };

	bool save(ostream&) const;
	bool load(istream&);
	
	long size() const { return mapVars.size(); }

	template<class T>
	bool set(const string name, const T v)
	{
		if (name.length())
		{
			string newv = StringUtil::to_string(v);
			auto it = mapVars.find(name);

			if ((it != mapVars.end()) && (newv.length() == 0))
         {
					mapVars.erase(it);
			}
			else if (it == mapVars.end())
         {
            mapVars[name] = newv;
            return true;
         }
         else
			{
				string old = it->second;
				if (old != newv)
            {
   				it->second = newv;
					return true;
            }
			}
		}
		else
			cerr << "Cannot assign var with empty name to value : " << v << endl;
		return false;
	}

	float getFloat(const string& name, float fdefault = 0) const;
	long getLong(const string& name) const;
	const string& getString(const string& name, const string &dflt="") const;

	void iterate(function<void(const string&, const string&)>);
	
  private:
	Vars(const Vars& orig);
	map<string, string> mapVars;

};

#endif /* VARS_HPP */

