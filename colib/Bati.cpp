#include "Bati.hpp"
#include "Colib.hpp"
#include <GL/glew.h>
#include <Color.h>
#include "Navette.hpp"
#include "Cloison.hpp"
#include "Column.hpp"

namespace Colib {

	Bati::Bati(const Colib* p)
	:
	pcolib(p),
	h(0) {
#define FACTOR 2
		navette = new Navette(this);
		h.setMaxVelocity(16*FACTOR);
		h.setMaxValue(10000);
		h.setMaxVelocityThreshold(10*FACTOR);
		h.setAccel(12*FACTOR);
	}
	
	Plateau* Bati::getPlateau()
	{
		return navette->getPlateau();
	}
	
	void Bati::setPlateau(Plateau* p)
	{
		navette->setPlateau(p);
	}

	bool Bati::render() {
		Color::red.render();
		h.update();

		int x1 = getXLeft();
		pilier(x1, 0);
		pilier(x1 + Column::DEPTH - THICKNESS, 0);
		pilier(x1, pcolib->getLength() - THICKNESS);
		pilier(x1 + Column::DEPTH - THICKNESS, pcolib->getLength() - THICKNESS);
		
		traverses();
		
		bool bRet = navette->render() || !h.targetReached();
		
		return bRet;
	}

	void Bati::pilier(int x, int z) {
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0, 0, -1);
		glVertex3i(x, 0, z);
		glVertex3i(x, pcolib->getHeight(), z);
		glVertex3i(x + THICKNESS, 0, z);
		glVertex3i(x + THICKNESS, pcolib->getHeight(), z);

		glNormal3i(1, 0, 0);
		glVertex3i(x + THICKNESS, 0, z + THICKNESS);
		glVertex3i(x + THICKNESS, pcolib->getHeight(), z + THICKNESS);

		glNormal3i(0, 0, 1);
		glVertex3i(x, 0, z + THICKNESS);
		glVertex3i(x, pcolib->getHeight(), z + THICKNESS);

		glNormal3i(-1, 0, 0);
		glVertex3i(x, 0, z);
		glVertex3i(x, pcolib->getHeight(), z);

		glEnd();
	}
	
	void Bati::traverses()
	{
		Color::dark_red.render();
		traverse(getXLeft());
		traverse(getXRight() - THICKNESS);
	}
	
	bool Bati::isReady()
	{
		if (navette->isReady())
			return true;
		return false;
	}
	
	bool Bati::moveTo(int z, int height)
	{
		if (navette->isReady())
		{
			h.setTarget(height);
			navette->centerOn(z);
			return true;
		}
		else
			cout << "Navette not ready" << endl;
		return false;
	}
	
	float Bati::getHeight() const
	{
		return h;
	}
	
	float Bati::getTopHeight() const
	{
		return getHeight() + THICKNESS;
	}
	
	int Bati::getLength() const
	{
		return pcolib->getLength();
	}

	int Bati::getXLeft() const
	{
		return Column::DEPTH + Cloison::THICKNESS;
	}
	
	int Bati::getXRight() const
	{
		return Column::DEPTH*2 + Cloison::THICKNESS;
	}
	
	void Bati::traverse(int x)
	{
		float H = getHeight();
		int Z2 = pcolib->getLength()-THICKNESS;
		glBegin(GL_TRIANGLE_STRIP);
		
		glNormal3i(0, -1, 0);
		glVertex3f(x, H, THICKNESS);
		glVertex3f(x, H, Z2);
		glVertex3f(x+THICKNESS, H, THICKNESS);
		glVertex3f(x+THICKNESS, H, Z2);
		
		glNormal3i(1, 0, 0);
		glVertex3f(x+THICKNESS, H+THICKNESS, THICKNESS);
		glVertex3f(x+THICKNESS, H+THICKNESS, Z2);
		
		glNormal3i(0, 1, 0);
		glVertex3f(x, H+THICKNESS, THICKNESS);
		glVertex3f(x, H+THICKNESS, Z2);
		
		glNormal3i(-1, 0, 0);
		glVertex3f(x, H, THICKNESS);
		glVertex3f(x, H, Z2);
		
		glEnd();

	}
}