#include <GL/glew.h>

#include "Navette.hpp"
#include "Bati.hpp"
#include "Plateau.hpp"
#include "Column.hpp"
#include "MotorSpeedHook.hpp"
#include <StringUtil.hpp>
#include "Colib.hpp"
#include "Wings.hpp"

namespace Colib
{
	Navette::Navette(Bati* bati) : pbati(bati), z(0), plateau(0), moving_col(0)
	{

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
		
		string s;
		s = "reverb 30:30 fm 0 100 am 0 100 square 30:30 triangle 100 hook";
		s = "fm 50 150 fm 80 100  triangle 100 sin 30 hook";
		s = "define sound { am 70 100 sinus 200:80 sq 300 triangle 400:10 } define motor { sound fm 99 101 sound sin 4 } fm 0 40 motor hook";
		
		speed_hook->changeSound(s);
		wings = new Wings();
		
	}
	
	Navette::~Navette()
	{
		delete wings;
		if (speed_hook)
		{
			cout << "DELETING HOOK" << endl;
			delete speed_hook;
		}
	}
	
	bool Navette::isAllStopped() const
	{
		bool ret = true;
		ret = ret && z.targetReached();
		ret = ret && wings->isAllStopped();
		if (plateau) ret = ret && plateau->isAllStopped();
		ret = ret && (moving_col==0);
		return ret;
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
			cerr << "ERROR : Setting plateau with existing one !" << endl;
			delete plateau;
		}
		if (p)
			cout << "Navette: GOT PLATEAU" << endl;
		plateau = p;
		wings->adjustFor(plateau);
	}
	
	bool Navette::render()
	{
		z.update();

#ifdef HAVE_SYNTH
		if (speed_hook)
			speed_hook->update(z.getVelocity());
#endif
		
		float Y = pbati->getTopHeight();
		float ytop = Y + HEIGHT_Y;
		float cz = z+LENGTH_Z/2.0;								// Centre navette Z
		float cx = (pbati->getXLeft()+pbati->getXRight())/2;	// Centre X

		wings->render(cx, ytop, cz, plateau, pbati);
		
		// Suite de put / get: les ailes sont déployées
		if (moving_col && wings->isAllStopped())
		{
			if (putting)
			{
				if (plateau != 0)
				{
					int xdest = pbati->pcolib->getCenterOfColumnX(putting_back);
					plateau->setTargetCenterX(xdest);
				}
			}
			else
			{
				Plateau* p = moving_col->getPlateau(etage_dest);
				if (p && plateau==0)
				{
					cout << "GOT PLATEAU" << endl;
					setPlateau(p);
					moving_col->setPlateau(etage_dest, 0);
					plateau->setTargetCenterX(pbati->getCenterX());
				}
			}
		}
		
		if (plateau)
		{
			plateau->renderAtCenter(ytop, cz);
			if (moving_col && plateau->isReady() && wings->isAllStopped())
			{
				if (putting)
				{
					cout << "PUT COMPLETED" << endl;
					moving_col->setPlateau(etage_dest, plateau);
					plateau = 0;
				}
				moving_col = 0;
			}
		}
		
		return !isAllStopped();
	}
	
	void Navette::centerOn(float zz, bool back)
	{
		if (plateau)
			back = !back;
		wings->adjustFor(0, back);
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
			putting_back = back;
			wings->adjustFor(plateau, !back);
			putting = true;
			// suite dans render (car attente de wings)
			return 0;
		}
		return "Pas de place ici";
	}
	
	const char* Navette::get(Column* col, int etage, bool back)
	{
		cout << "NAVETTE GET " << etage << endl;
		if (plateau)
			return "Navette occupée !";
		Plateau* p = col->getPlateau(etage);
		if (p==0)
			return "Alvéole vide !!! ";
		
		etage_dest = etage;

		cout << "SETTING MOVING COL IN PUT " << back <<  endl;
		wings->adjustFor(p, back);
		moving_col = col;
		putting = false;
		
		// Suite dans render (car attente de wings)
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