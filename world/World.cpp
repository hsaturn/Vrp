/* 
 * File:   World.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 20:47
 */

#include <GL/glew.h>

#include "Blocks/blocks.hpp"
#include "World.hpp"
#include "StringUtil.hpp"
#include "Map.h"

namespace World
{
	int X=4;
	int Z=4;

	World::WorldBuilder World::builder;
	
	World::World(const string& name, string &incoming)
	: Object(name)
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
	
	World::~World()
	{
		delete m_map;
	}

	void World::_render(bool resetTimer)
	{
		m_map->render();
	}
	
	bool World::_execute(Server*, string cmd, string incoming, const string& org)
	{
		bool bRet = false;
		
		if (cmd=="add")
			bRet = true;
		else if (cmd=="read")
		{
			m_map->readFile(incoming);
		}
		return bRet;
	}
	
	void World::_help(Help& help)
	{
		//help.add("world.add block_type ...");
	}
	
		
}

