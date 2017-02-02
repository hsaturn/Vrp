/* 
 * File:   Plateau.cpp
 * Author: francois
 * 
 * Created on 2 février 2017, 11:58
 */
#include "Plateau.hpp"
#include "Column.hpp"
#include "Color.h"
#include <GL/glew.h>

namespace Colib
{
	void Plateau::renderAtCenter(int cy, int cz)
	{
		x.update();
		int x1,x2,y1,y2,z1,z2;
		x1 = x-Column::DEPTH/2;
		x2 = x+Column::DEPTH/2;
		y1 = cy+1;
		y2 = cy+HEIGHT;
		z1 = cz-LENGTH/2;
		z2 = cz+LENGTH/2;
		
		Color::orange.render();
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3i(x1, y1, z1);
		glVertex3i(x2, y1, z1);
		glVertex3i(x2, y1, z2);
		glEnd();
	}
}

