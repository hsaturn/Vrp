/* 
 * File:   Solid.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 21:12
 */

#include "Solid.hpp"
#include <GL/glew.h>
#include <iostream>

using namespace std;

namespace RedStone
{
	Solid::Solid(string &incoming)
	{
	}
	
	void Solid::render(Coord c, const RedMap*)
	{
		Color::dark_green.render();
		
		cubeRender(c,true);
	}
}
