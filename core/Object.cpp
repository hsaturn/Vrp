/*
 * File:   Object.cpp
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 00:09
 */

#include "Object.h"
#include "StringUtil.hpp"
#include "ObjectBuilder.hpp"
#include <stdlib.h>
#include <Widget.h>
#include <sys/stat.h>

IRunnable::ExecResult Application::execute(Server* server, string cmd, string incoming, const string& org, CmdQueue& cmdQueue)
{
	if (StringUtil::preg_match("^[a-zA-Z]+[a-zA-Z0-9_]*=", cmd, false))
	{
		string name = StringUtil::getWord(cmd, '=');
		setVar(name, cmd);
		return IRunnable::EXEC_OK;
	}
	else if (StringUtil::preg_match("^[a-zA-Z][a-zA-Z0-9_]*\\?", cmd))
	{
		cmd.erase(cmd.length() - 1, 1);
		server->send("#OK " + getName() + '.' + cmd + "=" + getString(cmd));
		return IRunnable::EXEC_OK;
	}
	else if (cmd == "vars")
	{
		auto func = [this, server](const string& name, const string & value)
		{
			server->send(getName() + '.' + name + '=' + value);
		};

		vars.iterate(func);
		server->send("Vars count : " + StringUtil::to_string(vars.size()));
		return IRunnable::EXEC_OK;
	}
	else
		return _execute(server, cmd, incoming, org, cmdQueue);
}

void Application::drawHudText(const string& txt) const
{
	const char* message = txt.c_str();

	while (*message)
	{
		// TODO glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		message++;
	}
}

bool Application::render(bool resetTimer)
{
	bool bRet = false;
	if (isVisible())
	{
		glPushMatrix();
		glSetMatrix();
		bRet = _render(resetTimer);
		glPopMatrix();
	}
	return bRet;
}

void Application::glSetMatrix()
{
	float scale = getFloat("scale", 1);
	glTranslatef(getFloat("dx"), getFloat("dy"), getFloat("dz"));
	glScalef(
			getFloat("sx", 1) * scale,
			getFloat("sy", 1) * scale,
			getFloat("sz", 1) * scale);
}

void Application::help(Help& help)
{
	help.add("var=value   set var");
	help.add("var?        display var");
	help.add("dx/dy/dz    translate object");
	help.add("sx/sy/sz    scale object");
	help.add("vars        display vars of object");
	_help(help);
}

bool Application::isVisible()
{
	return true;
	return vars.getFloat("visible", 1);
}

float Application::eatFloat(string& buf) const
{
	string name = StringUtil::getWord(buf);
	if (name.length())
	{
		const string& v = vars.getString(name, name);
		if (v[0]=='.' || v[0]=='-' || v[0]=='+' || (v[0]>='0' && v[0]<='9'))
			return atof(v.c_str());
		cerr << "Not a float : " << v << endl; // TODO not visible in consoles
	}
	cerr << "Application::eatFloat, float expected (was parsing " << name << ") end of chain (" << buf << ")" << endl; // TODO not visible in console
	return 0;
}

int Application::getQuality() const
{
	int quality = getLong("quality");
	if (quality == 0)
	{
		string v = hwidgets::Widget::getVar("quality");
		if (v.length())
		{
			quality = atol(v.c_str());
		}
	}
	if (quality < 2) quality = 2;
	if (quality > 100) quality = 100;
	return quality;
}

string Application::getAppRsrcFileName(const string rel) const
{
	return builder->getRsrcFileName(rel);
}

string Application::getRsrcFileName(const string rel) const
{
	string rel2 = mname + '/' + rel;
	return builder->getRsrcFileName(rel2);
}

const string& Application::getAppClass() const
{
   return builder->getAppClass();
}