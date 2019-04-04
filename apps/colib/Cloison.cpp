#include "Cloison.hpp"
#include "Plateau.hpp"
#include <Color.h>

namespace Colib
{
	void Cloison::render(int x1, int x2, int height, int z, const vector<int>& heights)
	{
		int zz = z;
		
		for(int hollow=HOLLOW_Z; hollow>=-HOLLOW_Z; hollow -= 2*HOLLOW_Z)
		{
			glBegin(GL_TRIANGLE_STRIP);
			int h1 = 0;
			for(int h2 : heights)
			{
				if (h1)
				{
					Color::dark_gray.render();

					// plat rainure
					glVertex3i(x1, h1, zz);
					glVertex3i(x2, h1, zz);
					glVertex3i(x1, h1, zz+hollow);
					glVertex3i(x2, h1, zz+hollow);

					// fond rainure
					h1 += Plateau::THICKNESS;
					glVertex3i(x1, h1, zz+hollow);
					glVertex3i(x2, h1, zz+hollow);

					// plat haut rainure
					glVertex3i(x1, h1, zz);
					glVertex3i(x2, h1, zz);

					// cloison	
					Color::gray.render();
					glVertex3i(x1, h2, zz);
					glVertex3i(x2, h2, zz);

				}
				h1 = h2;
			}
			zz += THICKNESS_Z;
			glEnd();
			
		}
		
		Color::white.render();
		int x = x1;
		// Bords de cloison
		for(int i=0; i<=1; i++)
		{
			int h1 = 0;
			for(int h2: heights)
			{
				if (h1)
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3i(x, h1, z+THICKNESS_Z-HOLLOW_Z);
					glVertex3i(x, h1, z+HOLLOW_Z);

					h1 += Plateau::THICKNESS;
					glVertex3i(x, h1, z+THICKNESS_Z-HOLLOW_Z);
					glVertex3i(x, h1, z+HOLLOW_Z);
					glEnd();

					glBegin(GL_TRIANGLE_STRIP);
					glVertex3i(x, h1, z+THICKNESS_Z);
					glVertex3i(x, h1, z);
					glVertex3i(x, h2, z+THICKNESS_Z);
					glVertex3i(x, h2, z);
					glEnd();
				}

				h1 = h2;
			}
			if (x == x1) x = x2;
		}
		
		return;
	}
}
