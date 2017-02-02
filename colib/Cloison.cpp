#include "Cloison.hpp"
#include <GL/glew.h>
#include <Color.h>

namespace Colib
{
	void Cloison::render(int x1, int x2, int height, int z, const vector<int>& heights)
	{
		int h1 = heights[0];
		int z1 = z +THICKNESS/2;
		Color::gray.render();
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3i(x1, h1, z1);
		glVertex3i(x1, height, z1);
		glVertex3i(x2, h1, z1);
		glVertex3i(x2, height, z1);
		glEnd();
		
		for(int h : heights)
		{
			int dz=2*THICKNESS;
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3i(x1, h, z1-dz);
			glVertex3i(x1, h, z1+dz);
			glVertex3i(x2, h, z1-dz);
			glVertex3i(x2, h, z1+dz);
			glEnd();
		}
	}
}