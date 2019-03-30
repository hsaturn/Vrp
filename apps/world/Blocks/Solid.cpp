/* 
 * File:   Solid.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 21:12
 */

#include "Solid.hpp"
#include <iostream>

using namespace std;

namespace World
{
	Solid::Solid(string &incoming)
	{
	}
	
	void Solid::render(Coord c, const WorldApp*)
	{
		Color::dark_green.render();
		
		cubeRender(c,true);
	}
}
