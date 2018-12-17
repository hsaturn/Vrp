/* 
 * File:   Object.h
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 00:09
 */
#pragma once

#include <GL/glew.h>
#include "Server.h"
#include <string>
#include <list>

#include "Color.h"
#include "Help.h"
#include <StringUtil.hpp>

#include <map>
#include "application/Renderable.hpp"
#include "Vars.hpp"

using namespace std;

class ApplicationBuilder;

class Application : public Renderable
{
  public:
	friend class ApplicationBuilder;

	Application(const string& name) : mname(name) { };

	virtual void init() {};
	~Application() override { };

	/**
	 * 
	 * @return true if need to redisplay next frame
	 */
	bool render(bool resetTimer);

	virtual void renderHud() { };

	ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue& cmdQueue);
	void help(Help&);

	const string& getName() const
	{
		return mname;
	}

	float getFloat(const string& name, float fdefault = 0) const
	{
		return vars.getFloat(name, fdefault);
	}

	float eatFloat(string& buf) const;

	long getLong(const string& name) const
	{
		return vars.getLong(name);
	}

	const string& getString(const string& name, const string &deflt = "") const
	{
		return vars.getString(name, deflt);
	}

	void glSetMatrix();

	int getQuality() const;

	bool isVisible();

	bool saveVars(ostream& out)
	{
		vars.set("name", mname);
		return vars.save(out);
	}

	bool loadVars(istream& in)
	{
		bool ret = vars.load(in);
		if (ret)
			mname = vars.getString("name");
		return ret;
	}

	template<class T>
	void setVar(const string name, const T v)
	{
		if (vars.set(name, v))
		{
			_onVarChanged(name, vars.getString(name));
		}
	}

	string getAppRsrcFileName(const string rel) const;
	string getRsrcFileName(const string rel) const;

  protected:

	void drawHudText(const string & txt) const;

  private:
	Application(const Application & orig);
	string mname;
	Vars vars;
	ApplicationBuilder* builder; // Mainly to keep track of application class

};

#    define APPLICATION_BUILDER(appname, wclass) \
		class wclass##Builder : public ApplicationBuilder\
		{\
			public:\
				wclass##Builder() : ApplicationBuilder(appname) {}\
				Application* build(const string& name, string& incoming)\
				{ return new wclass(name, incoming); }\
		};


extern const Color* getColor(string& incoming);
extern string getWord(string& s, const string &sSeparators = " ");
extern list<string> cmdQueue;

