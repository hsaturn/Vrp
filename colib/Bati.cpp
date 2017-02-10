#include "Bati.hpp"
#include "Colib.hpp"
#include <GL/glew.h>
#include <Color.h>
#include <StringUtil.hpp>
#include "Navette.hpp"
#include "Cloison.hpp"
#include "Column.hpp"
#include "MotorSpeedHook.hpp"

namespace Colib {
	
	Bati::Bati(const Colib* p)
	:
	pcolib(p),
	h(0)
	{
		navette = new Navette(this);
		h.setMaxVelocity(MAX_SPEED);
		h.setMaxValue(10000);
		h.setMaxVelocityThreshold(10*FACTOR);
		h.setAccel(12*FACTOR);
		
		column_dest = 0;
		etage_dest = 0;
		
		static int number=0;
		string name="hook_bati_"+number++;
		hook_speed = new MotorSpeedHook(name, MAX_SPEED);
		string s="bati define sound { am 70 100 sinus 200:80 sq 300 triangle 400:10 sq 600:10 } reverb 30:30 { sound fm 10 100 sound " + name +" }";
		changeSound(s);
	}
	
	Bati::~Bati()
	{
		if (hook_speed)
			delete hook_speed;
		delete navette;
	}
	
	bool Bati::isAllStopped()
	{
		if (!h.targetReached())
			return false;
		return navette->isAllStopped();
	}

	void Bati::changeSound(string& incoming)
	{
		string what = StringUtil::getWord(incoming);
		stringstream in;
		in << incoming;
		if (what == "bati")
		{
			hook_speed->changeSound(in);
		}
		else if (what == "navette")
			navette->changeSound(in);
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
		
		if (hook_speed)
			hook_speed->update(h.getVelocity());

		int x1 = getXLeft();
		pilier(x1, 0);
		pilier(x1 + Column::DEPTH - THICKNESS, 0);
		pilier(x1, pcolib->getLength() - THICKNESS);
		pilier(x1 + Column::DEPTH - THICKNESS, pcolib->getLength() - THICKNESS);
		
		traverses();
		
		bool bRet = navette->render() || !h.targetReached();
		
		return bRet;
	}
	
	const char* Bati::put(bool back)
	{
		Column* col = pcolib->getColumn(column_dest, back);
		int xdest = pcolib->getCenterOfColumnX(back);
		return navette->put(col, etage_dest, xdest);
	}
	
	const char* Bati::get(bool back)
	{
		Column* col = pcolib->getColumn(column_dest, back);
		return navette->get(col, etage_dest);
	}
	
	void Bati::remove()
	{
		navette->remove();
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
	
	bool Bati::moveTo(int col_dest, int etage)
	{
		if (navette->isReady())
		{
			column_dest = col_dest;
			etage_dest = etage;
			cerr << "col_dest=" << col_dest << " / etage_dest=" << etage_dest << endl;
			
			h.setTarget(pcolib->getHeight(etage) -Navette::HEIGHT-Bati::THICKNESS);
			int z = pcolib->getCenterOfColumnZ(col_dest);
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