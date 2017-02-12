#include <GL/glew.h>

#include "Navette.hpp"
#include "Bati.hpp"
#include "Plateau.hpp"
#include "Column.hpp"
#include "MotorSpeedHook.hpp"
#include <StringUtil.hpp>
#include "Colib.hpp"

namespace Colib
{
	Navette::Navette(Bati* bati) : pbati(bati), z(0), plateau(0), moving_col(0)
	{
		z.setTarget(0);
		z.setMaxVelocity(20*FACTOR);
		z.setMaxValue(100000);
		z.setMaxVelocityThreshold(20*FACTOR);
		z.setAccel(10*FACTOR);
		
		static int number=0;
		string name = "nav_speed_"+StringUtil::to_string(number++);

		speed_hook = new MotorSpeedHook(name, MAX_SPEED);
		cout << "BUILDING SOUND GENERATOR" << endl;
		
		string s;
		s = "reverb 30:30 fm 0 100 am 0 100 square 30:30 triangle 100 hook";
		s = "fm 50 150 fm 80 100  triangle 100 sin 30 hook";
		s = "define sound { am 70 100 sinus 200:80 sq 300 triangle 400:10 } define motor { sound fm 99 101 sound sin 4 } fm 0 40 motor hook";
		
		speed_hook->changeSound(s);
	}
	
	Navette::~Navette()
	{
		if (speed_hook)
		{
			cout << "DELETING HOOK" << endl;
			delete speed_hook;
		}
	}
	
	bool Navette::isAllStopped() const
	{
		if (!z.targetReached())
			return false;
		if (moving_col)
			return false;
		if (plateau)
			return plateau->isAllStopped();
		return true;
	}

	
	void Navette::changeSound(string& sound)
	{
		if (speed_hook)
			speed_hook->changeSound(sound);
	}
	
	
	void Navette::setPlateau(Plateau* p)
	{
		if (plateau)
		{
			cerr << "ERROR : Destroying existing plateau !!! " << endl;
			delete plateau;
		}
		plateau = p;
	}
	
	bool Navette::render()
	{
		const int debord = 1;
		z.update();
	
#ifdef HAVE_SYNTH
		if (speed_hook)
			speed_hook->update(z.getVelocity());
#endif
		
		float Z1 = z;
		if (z < Bati::THICKNESS)
			z = Bati::THICKNESS;
		float Y = pbati->getTopHeight();
		
		Color::cyan.render();
		
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0 ,-1, 0);	// Bottom rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH_Z);
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y, Z1+LENGTH_Z);
		
		glNormal3i(1 ,0, 0);	// Right rectangle
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT_Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT_Y, Z1+LENGTH_Z);
		
		glNormal3i(0, 1, 0);	// Top rectangle
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT_Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT_Y, Z1+LENGTH_Z);
			
		glNormal3i(-1, 0, 0);	// Left rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH_Z);
		glEnd();
		
		Color::blue.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,-1);	// back rectangle
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT_Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT_Y, Z1);
		glEnd();
		
		Color::brown.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,1);	// front rectangle
		glVertex3f(pbati->getXRight()+debord, Y, Z1+LENGTH_Z);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH_Z);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT_Y, Z1+LENGTH_Z);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT_Y, Z1+LENGTH_Z);
		glEnd();
		
		if (plateau)
		{
			plateau->renderAtCenter(Y+HEIGHT_Y, z+LENGTH_Z/2);
			
			if (moving_col && plateau->isReady())
			{
				cout << "Fin déplacement x plateau ! " << plateau->getX() << endl;
				if (putting)
				{
					moving_col->setPlateau(etage_dest, plateau);
					plateau = 0;
				}
				moving_col = 0;
			}
		}
		
		return !isAllStopped();
	}
	
	void Navette::centerOn(float zz)
	{
		z.setTarget(zz-Navette::LENGTH_Z/2);
	}
	
	const char* Navette::put(Column* col, int etage, bool back)
	{
		if (!isAllStopped())
			return "Attendre l'arrêt complet";
		if (plateau==0)
			return "Navette vide !";
		
		if (col->hasRoomFor(etage, plateau))
		{
			etage_dest = etage;
			moving_col = col;
			putting = true;
			int xdest = pbati->pcolib->getCenterOfColumnX(back);
			plateau->setTargetX(xdest);
			cout << "Destination etage : " << col << '.' << etage << endl;
			cout << plateau->getMovingCoord() << endl;
			return 0;
		}
		return "Pas de place ici";
	}
	
	const char* Navette::get(Column* col, int etage)
	{
		cout << "get " << col << '.' << etage << endl;
		if (plateau)
			return "Navette occupée !";
		if (col->getPlateau(etage)==0)
			return "Alvéole vide !!! ";
		
		moving_col = col;
		plateau = moving_col->getPlateau(etage);
		moving_col->setPlateau(etage, 0);
		putting = false;
		plateau->setTargetX((pbati->getXLeft()+pbati->getXRight())/2);
		return "";
	}
	
	void Navette::remove()
	{
		if (plateau)
		{
			delete plateau;
			plateau = 0;
			moving_col = 0;
		}
	}

	
	bool Navette::isReady() const
	{
		if (moving_col)
			return false;
		else if (plateau)
			return plateau->isReady();
		return true;
	}
}