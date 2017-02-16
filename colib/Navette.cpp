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
	const Model* Navette::support_plateau_left=0;
	const Model* Navette::support_plateau_right=0;
	
	Navette::Navette(Bati* bati) : pbati(bati), z(0), plateau(0), moving_col(0)
	{
		
		const int MAX_SPEED = 20.0;
		wings_z.setValue(15);
		wings_z.setTarget(15);		// half length_z of current plateau
		wings_z.setAccel(MAX_SPEED*4);
		wings_z.setMaxValue(9999);
		wings_z.setMinValue(-9999);
		wings_z.setPositionTolerance(0.01);
		wings_z.setMaxVelocityThreshold(MAX_SPEED);
		wings_z.setMaxVelocity(MAX_SPEED);
		// @TODO SOUND ?
		
		z.setTarget(0);
		z.setMaxVelocity(20*FACTOR);
		z.setMinValue(-10000);
		z.setMaxValue(100000);
		z.setPositionTolerance(0.1);
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
		
		const float ep = 0.5;	// epaisseur bords support plateau
		if (support_plateau_left == 0)
		{
			support_plateau_left = Model::get("nav_support_plateau_left");
			if (support_plateau_left)
			{
				auto low = support_plateau_left->getMinCoord();
				auto hig = support_plateau_left->getMaxCoord();

				offset_support_left[0] = (-hig[0]-low[0])/2;
				offset_support_left[1] = -low[1] - ep;
				offset_support_left[2] = -hig[2] + ep + 0.1;	// 1mm d'écart plateau / support
			}
		}
		
		if (support_plateau_right == 0)
		{
			support_plateau_right = Model::get("nav_support_plateau_right");
			
			if (support_plateau_right)
			{
				auto low = support_plateau_right->getMinCoord();
				auto hig = support_plateau_right->getMaxCoord();

				offset_support_right[0] = (-hig[0]-low[0])/2;
				offset_support_right[1] = -low[1] - ep;
				offset_support_right[2] = -low[2] - ep - 0.1;
			}
		}
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
		if (!wings_z.targetReached())
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
		z.update();

#ifdef HAVE_SYNTH
		if (speed_hook)
			speed_hook->update(z.getVelocity());
#endif
		
		float Y = pbati->getTopHeight();
		float YTOP = Y + HEIGHT_Y;
		float cz = z+LENGTH_Z/2.0;								// Centre navette Z
		float cx = (pbati->getXLeft()+pbati->getXRight())/2;	// Centre X

		if (support_plateau_left || support_plateau_right)
		{
			if (plateau)
				wings_z.setTarget(plateau->getWidth()/2);
			wings_z.update();
		}
		
		if (support_plateau_left)
		{
			glPushMatrix();
			glTranslatef(cx+offset_support_left[0], YTOP+offset_support_left[1], cz+wings_z+offset_support_left[2]);
			support_plateau_left->render();
			glPopMatrix();
		}
		if (support_plateau_right)
		{
			glPushMatrix();
			glTranslatef(cx+offset_support_right[0], YTOP+offset_support_right[1], cz-wings_z+offset_support_right[2]);
			support_plateau_right->render();
			glPopMatrix();
		}

		// Suite de get: les ailes sont déployées
		if (moving_col && putting==false && wings_z.targetReached() && plateau==false)
		{
			Plateau* p = moving_col->getPlateau(etage_dest);
			if (p)
			{
				plateau = p;
				cerr << "OK PLATEAU" << endl;
				moving_col->setPlateau(etage_dest, 0);
				plateau->setTargetX((pbati->getXLeft()+pbati->getXRight())/2);
			}
		}
		
		if (plateau)
		{
			plateau->renderAtCenter(YTOP, cz);
			if (moving_col && plateau->isReady())
			{
				cout << "Fin déplacement x plateau ! " << plateau->getX() << endl;
				if (putting)
				{
					moving_col->setPlateau(etage_dest, plateau);
					plateau = 0;
				}
				cout << "SETTING MOVING COL TO 0" << endl;
				moving_col = 0;
			}
		}
		
		return !isAllStopped();
	}
	
	void Navette::centerOn(float zz)
	{
		
		cout << "CENTER " << zz << endl;
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
		cout << "NAVETTE GET " << etage << endl;
		if (plateau)
			return "Navette occupée !";
		Plateau* p = col->getPlateau(etage);
		if (p==0)
			return "Alvéole vide !!! ";
		
		etage_dest = etage;
		wings_z.setTarget(p->getWidth()/2);

		cout << "SETTING MOVING COL IN PUT " << endl;
		moving_col = col;
		putting = false;
		
		// Suite dans render (...)
		// quand wings_z reached target
		return 0;
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