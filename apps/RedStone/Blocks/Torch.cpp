/* 
 * File:   Torch.cpp
 * Author: francois
 * 
 * Created on 6 avril 2016, 15:37
 */

#include "Torch.hpp"

namespace RedStone
{

  Torch::Torch(string& incoming)
  : side(SOUTH) { }

  void Torch::render(Coord c, const RedMap*)
  {
	  const float d = 0.1;
	  Color::red.render();
	  glBegin(GL_QUADS);
	  glVertex3f(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 - d);
	  glVertex3f(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 - d);
	  glVertex3f(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 + d);
	  glVertex3f(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 + d);
	  glEnd();

	  Coord start;
	  switch (side)
		  {
	  case VERTICAL:
		  start = Coord(c.x() + 0.5, c.y(), c.z() + 0.5);
		  break;

	  case SOUTH:
		  start = Coord(c.x() + 0.5, c.y()+0.1, c.z() + 1);
		  break;

	  case NORTH:
		  start = Coord(c.x() + 0.5, c.y()+0.1, c.z());
		  break;

	  case WEST:
		  start = Coord(c.x() - 1, c.y()+0.1, c.z() + 0.5);
		  break;

	  case EAST:
		  start = Coord(c.x() + 1, c.y()+0.1, c.z() + 0.5);
		  break;
		  }
	  
	  Coord pa(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 - d);
	  Coord pb(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 - d);
	  Coord pc(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 + d);
	  Coord pd(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 + d);
	  
	  Color::brown.render();
	  glBegin(GL_TRIANGLE_FAN);
	  start.render();
	  pa.render();
	  pb.render();
	  pc.render();
	  pd.render();
	  pa.render();
	  glEnd();
	  
	  Color::black.render();
	  glBegin(GL_LINE_LOOP);
	  glVertex3f(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 - d);
	  glVertex3f(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 - d);
	  glVertex3f(c.x() + 0.5 + d, c.y() + 0.5, c.z() + 0.5 + d);
	  glVertex3f(c.x() + 0.5 - d, c.y() + 0.5, c.z() + 0.5 + d);
	  glEnd();
	  
	  glBegin(GL_LINES);
	  start.render();
	  pa.render();
	  start.render();
	  pb.render();
	  start.render();
	  pc.render();
	  start.render();
	  pd.render();
	  glEnd();
  }
}
