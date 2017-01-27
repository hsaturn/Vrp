/* 
 * File:   RedStone.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 20:36
 */

#include "RedStone.hpp"

namespace RedStone
{
	RedStone::RedStoneBuilder RedStone::builder;
	
	RedStone::RedStone(const string& name, string& incoming)
	: Object(name)
	{
		
	}
	
	bool RedStone::_execute(Server*, string cmd, string incoming, const string& org, CmdQueue&)
	{
		return false;
	}
	
	void RedStone::_help(Help&)
	{
		
	}
	
	bool RedStone::_render(bool resetTimer)
	{
		return false;
	}

}
