#include "Bati.hpp"
#include "Colib.hpp"
#include <GL/glew.h>
#include <Color.h>

namespace Colib
{
	
	Bati::Bati(const Colib* p)
	:
		pcolib(p),
		h(0)
	{
		
	}
	
void Bati::render()
{
	Color::red.render();
	h.update();
	
	int x1=Colib::COL_WIDTH+Colib::CLO_THICK;
	pilier(x1, 0);
	pilier(x1+Colib::COL_WIDTH-BATI_THICK, 0);
	pilier(x1, pcolib->getLength()-BATI_THICK);
	pilier(x1+Colib::COL_WIDTH-BATI_THICK, pcolib->getLength()-BATI_THICK);
}

void Bati::pilier(int x, int z)
{
	glBegin(GL_QUAD_STRIP);
	glVertex3i(x,0,z);
	glVertex3i(x, pcolib->getHeight(), z);
	glVertex3i(x+BATI_THICK, 0,z);
	glVertex3i(x+BATI_THICK, pcolib->getHeight(), z);
	
	glVertex3i(x+BATI_THICK, 0, z+BATI_THICK);
	glVertex3i(x+BATI_THICK, pcolib->getHeight(), z+BATI_THICK);
	
	glVertex3i(x, 0, z+BATI_THICK);
	glVertex3i(x, pcolib->getHeight(), z+BATI_THICK);
	
	glVertex3i(x,0,z);
	glVertex3i(x, pcolib->getHeight(),z);
		
	glEnd();
}
}