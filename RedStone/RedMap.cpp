/* 
 * File:   RedMap.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 20:47
 */

#include <GL/glew.h>

#include "Blocks/blocks.hpp"
#include "RedMap.hpp"

namespace RedStone
{
	int X=4;
	int Z=4;

	RedMap::RedMapBuilder RedMap::builder;
	
	RedMap::RedMap(const string& name, string &incoming)
	: Object(name)
	{
		for(int x=-X; x<X; x++)
			for(int z=-Z; z<Z; z++)
			{
				string s("dirt");
				Coord c(x,0,z);
				ground[c]=new Solid(s);
			}
		
		string s;
		Coord c(0,1,0);
		tiles[c]=new Torch(s);
		tiles[Coord(0,1,1)]=new Solid(s);
		tiles[Coord(0,2,1)]=new Solid(s);
		
		cerr << "REDMAP SIZE=" << ground.size() << endl;
	}
	
	RedMap::~RedMap()
	{
		for(auto it: tiles)
			delete it.second;
	}

	bool RedMap::_render(bool resetTimer)
	{
		for(auto it : ground)
			it.second->render(it.first, this);
		for(auto it : tiles)
			it.second->render(it.first, this);
		return false;
		
		glBegin(GL_QUADS);
		Color::dark_green.render();
		glVertex3i(-X,0, Z);
		glVertex3i( X,0, Z);
		glVertex3i( X,0,-Z);
		glVertex3i(-X,0,-Z);
		glEnd();
		
		glBegin(GL_LINES);
		Color::black.render();
		
		for(int x=-X; x<X; x++)
		{
			glVertex3f(x,0.01,-Z);
			glVertex3f(x,0.01,Z);
		}
		for(int z=-Z; z<Z; z++)
		{
			glVertex3f(X,0.01,z);
			glVertex3f(-X,0.01,z);
		}
		glEnd();
		return FALSE;
	}
	
	Object::ExecResult RedMap::_execute(Server*, string cmd, string incoming, const string& org, CmdQueue&)
	{
		ExecResult ret = FALSE;
		
		if (cmd=="add")
			ret = TRUE;
		return ret;
	}
	
	void RedMap::_help(Help& help)
	{
		help.add("redmap.add block_type ...");
	}
	
	Block* RedMap::getTile(const Coord& c) const
	{
		auto it = tiles.find(c);
		if (it==tiles.end()) return 0;
		return it->second;
	}
		
}


