/* 
 * File:   Object.h
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 00:09
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include "Server.h"
#include <string>
#include <list>
#include "Color.h"
#include "Help.h"
#include "ObjectBuilder.hpp"

#include <map>

using namespace std;

class Object {
public:
	Object(const string& name) : mname(name){};
	virtual ~Object(){};
	
	/**
	 * 
	 * @return true if need to redisplay next frame
	 */
	bool render(bool resetTimer);
	virtual void renderHud(){};
	
	bool execute(Server*, string cmd, string incoming, const string& org);
	void help(Help&);
	
	const string& getName() const { return mname; }
	
	float getFloat(const string& name, float fdefault=0) const;
	float eatFloat(string& buf) const;
	long getLong(const string& name) const;
	const string& getString(const string& name) const;
	
	void glSetMatrix();
	
	int getQuality() const;
	
	void setVar(const string& name, const string value) { mapVars[name]=value; }
	bool isVisible();
	
	
protected:
	virtual bool _execute(Server*, string cmd, string incoming, const string& org){ return false; };
	virtual void _help(Help&){};
	
	/**
	 * @return true if need to redisplay asap
	 */
	virtual bool _render(bool resetTimer) = 0;

	void drawHudText(const string& txt) const;
	
private:
	Object(const Object& orig);
	string mname;
	map<string,string> mapVars;

};

extern const Color* getColor(string& incoming);
extern string getWord(string& s, const string &sSeparators = " ");
extern list<string> cmdQueue;
#endif	/* OBJECT_H */

