#include "Wings.hpp"

#include "model/Model.hpp"
#include "Plateau.hpp"
#include "Vrp/Planet.hpp"
#include "Column.hpp"
#include "Bati.hpp"

namespace Colib
{
	const Model* Wings::support_plateau_left=0;
	const Model* Wings::support_plateau_right=0;
	const Model* Wings::pusher = 0;
	const Model* Wings::pusher_support = 0;
	float Wings::pusher_support_offset_y = 0;
	
	float Wings::offset_support_left[3];
	float Wings::offset_support_right[3];
	
	Wings::Wings()
	{
		float MAX_SPEED = 20.0;
		wings_z.setValue(15);
		wings_z.setTarget(15);		// half length_z of current plateau
		wings_z.setAccel(MAX_SPEED*4);
		wings_z.setMaxValue(9999);
		wings_z.setMinValue(-9999);
		wings_z.setPositionTolerance(0.01);
		wings_z.setMaxVelocityThreshold(MAX_SPEED);
		wings_z.setMaxVelocity(MAX_SPEED);
		
		// Poussoir
		pusher_x.setValue(1);
		pusher_x.setTarget(1);		
		pusher_x.setAccel(MAX_SPEED);
		pusher_x.setMaxValue(1);
		pusher_x.setMinValue(-1);
		pusher_x.setPositionTolerance(0.01);
		pusher_x.setMaxVelocityThreshold(0.1);
		pusher_x.setMaxVelocity(2);

		pusher_x.setValue(0);
		pusher_x.setTarget(0);		
		pusher_x.setAccel(MAX_SPEED);
		pusher_x.setMaxValue(1);
		pusher_x.setMinValue(-1);
		pusher_x.setPositionTolerance(0.01);
		pusher_x.setMaxVelocityThreshold(0.1);
		pusher_x.setMaxVelocity(2);
		
		loadModels();
	}
	

	void Wings::loadModels()
	{
		static bool done=false;
		if (done) return;
		
		const float ep = 0.5;	// epaisseur bords support plateau
		support_plateau_left = Model::get("nav_support_plateau_left");
		if (support_plateau_left)
		{
			auto low = support_plateau_left->getMinCoord();
			auto hig = support_plateau_left->getMaxCoord();

			offset_support_left[0] = (-hig[0]-low[0])/2;
			offset_support_left[1] = -low[1] - ep;
			offset_support_left[2] = -hig[2] + ep + 0.1;	// 1mm d'écart plateau / support
		}
		
		support_plateau_right = Model::get("nav_support_plateau_right");

		if (support_plateau_right)
		{
			auto low = support_plateau_right->getMinCoord();
			auto hig = support_plateau_right->getMaxCoord();

			offset_support_right[0] = (-hig[0]-low[0])/2;
			offset_support_right[1] = -low[1] - ep;
			offset_support_right[2] = -low[2] - ep - 0.1;
		}
		
		pusher = Model::get("nav_pusher");
		checkCentered(pusher, "nav_pusher");
		
		pusher_support = Model::get("nav_pusher_support");
		
		if (pusher_support)
		{
			checkCentered(pusher_support, "nav_pusher_support");
			pusher_support_offset_y = pusher_support->getHeight() + pusher_support->getMinCoord()[1];
		}
	}
	
	void Wings::checkCentered(const Model* p, const char* name)
	{
		if (p)
		{
			if (
				(abs(p->getMinCoord()[0]-p->getMaxCoord()[0])>0.5) ||
				(abs(p->getMinCoord()[2]-p->getMaxCoord()[2])>0.5)
				)
				cerr << "WARNING: Wings " << name << " model must be x & z centered." << endl;
		}
	}
	
	bool Wings::isAllStopped()
	{
		return wings_z.targetReached() && pusher_x.targetReached() && pusher_offset_x.targetReached();
	}
	
	bool Wings::render(float cx, float ytop, float cz, const Plateau* plateau, const Bati* pbati)
	{
		float delta_x = 0;
		if (plateau)
		{
			if (plateau->getXDest()==pbati->getCenterX())	// get
			{
				if (pusher_x>1)	// get from front
				{
					cout << " FROM FRONT" << endl;
					delta_x = -(plateau->getX()-plateau->getXDest());
				}
				else // get from back
				{
					cout << " FROM BACK" << endl;
					delta_x = (plateau->getX()-plateau->getXDest());
				}
			}
			else // put
				delta_x = (plateau->getX()-pbati->getCenterX());
			pusher_offset_x.setValue(delta_x);
		}
		else
			pusher_offset_x.setTarget(0);

		wings_z.update();
		
		if (pusher_support)
		{
			float y = ytop - pusher_support_offset_y;
			
			if (plateau && !pusher_x.targetReached())
			{
				if (pusher)
					y -= pusher->getHeight();
				else
					y -= 2.0;
			}
			glPushMatrix();
			glTranslatef(cx, y, cz);
			pusher_support->render();
			if (pusher)
			{
				y = pusher_support_offset_y-pusher->getMinCoord()[1];
				glTranslatef(pusher_offset_x+pusher_x*(pusher->getLengthX()+Column::DEPTH_X/2), y, 0);
				pusher->render();
			}
			glPopMatrix();
		}
		
		pusher_x.update();
		pusher_offset_x.update();
		
		if (support_plateau_left || support_plateau_right)
		{
		
			if (support_plateau_left)
			{
				glPushMatrix();
				glTranslatef(cx+offset_support_left[0], ytop+offset_support_left[1], cz+wings_z+offset_support_left[2]);
				support_plateau_left->render();
				glPopMatrix();
			}
			if (support_plateau_right)
			{
				glPushMatrix();
				glTranslatef(cx+offset_support_right[0], ytop+offset_support_right[1], cz-wings_z+offset_support_right[2]);
				support_plateau_right->render();
				glPopMatrix();
			}
		}
		else
			cerr << "ERROR : Wings no support !" << endl;
		return !wings_z.targetReached();
	}
	
	void Wings::adjustFor(const Plateau* p, bool back)
	{
		adjustFor(p);
		if (back)
			pusher_x.setTarget(-1);
		else
			pusher_x.setTarget(1);
		cout << "PUSHER TARGET " << pusher_x.getTarget() << endl;
	}
	
	loadvoid Wings::adjustFor(const Plateau* p)
	{
		if (p) wings_z.setTarget(p->getWidth()/2);
	}


}	

