/* 
 * File:   Coord.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 21:24
 */
#include <GL/glew.h>
#include "Coord.hpp"


namespace World
{

  Coord::Coord()
  {
	  key = 0;
	  _x=0;
	  _y=0;
	  _z=0;
  }

  Coord::Coord(Type cx, Type cy, Type cz)
  {
	  _x=cx;
	  _y=cy;
	  _z=cz;
	  key=_x+1000.0*_y+1000.0*_z;
  }

  void Coord::render()
  {
	  glVertex3f(_x, _y, _z);
  }

}
