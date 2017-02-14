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
#include <StringUtil.hpp>

#include <map>

using namespace std;

typedef list<string> CmdQueue;

class Object {
public:
	typedef enum ExecResult
	{
		FALSE,	// Unknown
		FAILED,	// Known, execution failed
		TRUE,	// Execution ok
		WAITING	// Known but cannot run now
	} ExecResult;
	
	Object(const string& name) : mname(name){};
	virtual ~Object(){};
	
	/**
	 * 
	 * @return true if need to redisplay next frame
	 */
	bool render(bool resetTimer);
	virtual void renderHud(){};
	
	ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue& cmdQueue);
	void help(Help&);
	
	const string& getName() const { return mname; }
	
	float getFloat(const string& name, float fdefault=0) const;
	float eatFloat(string& buf) const;
	long getLong(const string& name) const;
	const string& getString(const string& name) const;
	
	void glSetMatrix();
	
	int getQuality() const;
	
	template<class T>
	void setVar(const string& name, const T value)
	{
		if (name.length())
			mapVars[name]=StringUtil::to_string(value);
		else
			cerr << "Cannot assign var with empty name to value : " << value << endl;
	}

	bool isVisible();
	
	bool saveVars(ostream&) const;
	bool loadVars(istream&);
protected:
	virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&)=0;
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

