/*
 * File:   World.cpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */


#include "Blocks/blocks.hpp"
#include "WorldApp.hpp"
#include "StringUtil.hpp"
#include "Map.h"

namespace World
{
	int X=4;
	int Z=4;

	WorldApp::WorldAppBuilder WorldApp::builder;

	WorldApp::WorldApp(const string& name, string &incoming)
	: Application(name)
	{
		int width = StringUtil::getLong(incoming);
		int length = StringUtil::getLong(incoming);
		float scale = StringUtil::getFloat(incoming);

		if (width<10) width=10;
		if (width>200) width=200;
		if (length<10) length=10;
		if (length>200) length=200;
		if (scale<-10) scale=-10;
		if (scale>10) scale=10;
		if (scale == 0) scale = 1.0f;

		m_map = new Map(width, length, scale);
		cerr << "WORLD scale = " << scale << endl;
	}

	WorldApp::~WorldApp()
	{
		delete m_map;
	}

	bool WorldApp::_render(bool resetTimer)
	{
		bool bRet;
		bRet = m_map->render();
		return bRet;
	}

	IRunnable::ExecResult WorldApp::_execute(Server*, string cmd, string incoming, const string& org, CmdQueue&)
	{
		ExecResult ret = EXEC_UNKNOWN;

		if (cmd=="add")
			ret = EXEC_OK;
		else if (cmd=="read")
		{
			m_map->readFile(incoming);
			ret = EXEC_OK;
		}
		return ret;
	}

	void WorldApp::_help(Help& help) const
	{
      help.add("add block_type");
      help.add("read filename");
		//help.add("world.add block_type ...");
	}


}


