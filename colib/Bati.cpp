#include "Bati.hpp"
#include "Colib.hpp"
#include <GL/glew.h>
#include <Color.h>
#include "Navette.hpp"

namespace Colib {

	Bati::Bati(const Colib* p)
	:
	pcolib(p),
	h(0) {
		navette = new Navette(this);
		h.setMaxVelocity(16);
		h.setMaxValue(100);
		h.setMaxVelocityThreshold(10);
		h.setAccel(12);
	}

	bool Bati::render() {
		Color::red.render();
		h.update();

		int x1 = getXLeft();
		pilier(x1, 0);
		pilier(x1 + Colib::COL_WIDTH - BATI_THICK, 0);
		pilier(x1, pcolib->getLength() - BATI_THICK);
		pilier(x1 + Colib::COL_WIDTH - BATI_THICK, pcolib->getLength() - BATI_THICK);
		
		traverses();
		
		bool bRet = navette->render() || !h.targetReached();
		
		return bRet;
	}

	void Bati::pilier(int x, int z) {
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0, 0, -1);
		glVertex3i(x, 0, z);
		glVertex3i(x, pcolib->getHeight(), z);
		glVertex3i(x + BATI_THICK, 0, z);
		glVertex3i(x + BATI_THICK, pcolib->getHeight(), z);

		glNormal3i(1, 0, 0);
		glVertex3i(x + BATI_THICK, 0, z + BATI_THICK);
		glVertex3i(x + BATI_THICK, pcolib->getHeight(), z + BATI_THICK);

		glNormal3i(0, 0, 1);
		glVertex3i(x, 0, z + BATI_THICK);
		glVertex3i(x, pcolib->getHeight(), z + BATI_THICK);

		glNormal3i(-1, 0, 0);
		glVertex3i(x, 0, z);
		glVertex3i(x, pcolib->getHeight(), z);

		glEnd();
	}
	
	void Bati::traverses()
	{
		Color::dark_red.render();
		traverse(getXLeft());
		traverse(getXRight() - BATI_THICK);
	}
	
	void Bati::moveTo(int col, int h_percent)
	{
		h.setTarget(h_percent);
		navette->moveTo(col);
	}
	
	float Bati::getHeight() const
	{
		return h * ((float)pcolib->getHeight()-Bati::BATI_THICK)/100.0;
	}
	
	float Bati::getTopHeight() const
	{
		return getHeight() + BATI_THICK;
	}
	
	int Bati::getLength() const
	{
		return pcolib->getLength();
	}

	int Bati::getXLeft() const
	{
		return Colib::COL_WIDTH + Colib::CLO_THICK;
	}
	
	int Bati::getXRight() const
	{
		return Colib::COL_WIDTH*2 + Colib::CLO_THICK;
	}
	
	void Bati::traverse(int x)
	{
		float H = getHeight();
		int Z2 = pcolib->getLength()-BATI_THICK;
		glBegin(GL_TRIANGLE_STRIP);
		
		glNormal3i(0, -1, 0);
		glVertex3f(x, H, BATI_THICK);
		glVertex3f(x, H, Z2);
		glVertex3f(x+BATI_THICK, H, BATI_THICK);
		glVertex3f(x+BATI_THICK, H, Z2);
		
		glNormal3i(1, 0, 0);
		glVertex3f(x+BATI_THICK, H+BATI_THICK, BATI_THICK);
		glVertex3f(x+BATI_THICK, H+BATI_THICK, Z2);
		
		glNormal3i(0, 1, 0);
		glVertex3f(x, H+BATI_THICK, BATI_THICK);
		glVertex3f(x, H+BATI_THICK, Z2);
		
		glNormal3i(-1, 0, 0);
		glVertex3f(x, H, BATI_THICK);
		glVertex3f(x, H, Z2);
		
		glEnd();

	}
}