#include "Wings.hpp"

#include "model/Model.hpp"
#include "Plateau.hpp"
#include "Vrp/Planet.hpp"
#include "Column.hpp"
#include "Colib.hpp"
#include "Bati.hpp"

namespace Colib {
	const Model* Wings::support_plateau_left = 0;
	const Model* Wings::support_plateau_right = 0;
	const Model* Wings::pusher = 0;
	const Model* Wings::pusher_support = 0;
	float Wings::pusher_support_offset_y = 0;

	float Wings::offset_support_left[3];
	float Wings::offset_support_right[3];

	Wings::Wings() {
		float MAX_SPEED = 20.0;
		wings_z.setValue(15);
		wings_z.setTarget(15); // half length_z of current plateau
		wings_z.setAccel(MAX_SPEED * 4);
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

	void Wings::loadModels() {
		static bool done = false;
		if (done) return;

		const float rebord_y = 5; // hauteur du rebord des ailes
		const float esp_z = 0.5; // jeu fonctionnel bord des ailes / plateau
		support_plateau_left = Model::get("nav_support_plateau_left");
		if (support_plateau_left) {
			auto low = support_plateau_left->getMinCoord();
			auto hig = support_plateau_left->getMaxCoord();

			offset_support_left[0] = (-hig[0] - low[0]) / 2;
			offset_support_left[1] = rebord_y - hig[1];
			offset_support_left[2] = -hig[2] + esp_z + 0.1; // 1mm d'écart plateau / support
		}

		support_plateau_right = Model::get("nav_support_plateau_right");

		if (support_plateau_right) {
			auto low = support_plateau_right->getMinCoord();
			auto hig = support_plateau_right->getMaxCoord();

			float avoid_flick = 0.005;	// Avoid flickering because left & right support overlap each other
			offset_support_right[0] = (-hig[0] - low[0]) / 2+avoid_flick;
			offset_support_right[1] = rebord_y - hig[1]+avoid_flick;	
			offset_support_right[2] = -low[2] - esp_z - 0.1+avoid_flick;
		}

		pusher = Model::get("nav_pusher");
		checkCentered(pusher, "nav_pusher");

		pusher_support = Model::get("nav_pusher_support");

		if (pusher_support) {
			checkCentered(pusher_support, "nav_pusher_support");
			pusher_support_offset_y = pusher_support->getHeight() + pusher_support->getMinCoord()[1];
		}
	}

	void Wings::checkCentered(const Model* p, const char* name) {
		if (p) {
			if (
				(abs(p->getMinCoord()[0] - p->getMaxCoord()[0]) > 0.5) ||
				(abs(p->getMinCoord()[2] - p->getMaxCoord()[2]) > 0.5)
				)
				cerr << "WARNING: Wings " << name << " model must be x & z centered." << endl;
		}
	}

	bool Wings::isAllStopped() {
		return wings_z.targetReached() && pusher_x.targetReached();
	}

	string round10(float x, float rount=100) {
		return StringUtil::to_string((int) (x * rount) / rount) + ' ';
	}

	bool Wings::render(float cx, float ytop, float cz, const Plateau* plateau, const Bati* pbati) {

		if (plateau && plateau == adjust && pusher_x.targetReached()) {
			float delta_x = 0;
			float px = plateau->getX();
			delta_x = plateau->getX() - pusher_offset_x;
			pusher_offset_x = px;
			pusher_x = pusher_x + delta_x * 2.0 / (pusher->getLengthX() + Column::DEPTH_X);
		} else {
			pusher_x.update();
			if (abs(pusher_x)>1)
			{
				colog("PX", round10(pusher_x, 100));
				if (pusher_x>1)
					pusher_x.setTarget(1);
				else
					pusher_x.setTarget(-1);
			}
		}

		if (abs(pusher_x)>1)
			colog("PX", round10(pusher_x, 100));

		wings_z.update();

		if (pusher_support) {
			float y = ytop - pusher_support_offset_y;

			if (plateau && !pusher_x.targetReached()) {
				if (pusher)
					y -= pusher->getHeight();
				else
					y -= 2.0;
			}
			
			glPushMatrix();
			glTranslatef(cx, y, cz);
			pusher_support->render();
			if (pusher) {
				y = pusher_support_offset_y - pusher->getMinCoord()[1];
				y -= 0.5;	// Hauteur de la patte centrale inférieure du pusher (...)
				glTranslatef(pusher_x * (pusher->getLengthX() + Column::DEPTH_X) / 2.0, y, 0);
				pusher->render();
			}
			glPopMatrix();
		}

		if (support_plateau_left || support_plateau_right) {

			if (support_plateau_left) {
				glPushMatrix();
				glTranslatef(cx + offset_support_left[0], ytop + offset_support_left[1], cz + wings_z + offset_support_left[2]);
				support_plateau_left->render();
				glPopMatrix();
			}
			if (support_plateau_right) {
				glPushMatrix();
				glTranslatef(cx + offset_support_right[0], ytop + offset_support_right[1], cz - wings_z + offset_support_right[2]);
				support_plateau_right->render();
				glPopMatrix();
			}
		} else
			cerr << "ERROR : Wings no support !" << endl;
		return !wings_z.targetReached();
	}

	void Wings::adjustFor(const Plateau* p, bool back) {
		cout << "ADJUST FOR PLATEAU BACK " << p << back << endl;
		adjustFor(p);
		cout << "PUSH TARGET PREV / CURRENT " << round10(pusher_x.getTarget()) << " / ";
		if (back)
			pusher_x.setTarget(-1);
		else
			pusher_x.setTarget(1);
		cout << round10(pusher_x.getTarget()) << " reached ? " << pusher_x.targetReached() << endl;
	}

	void Wings::adjustFor(const Plateau* p) {
		cout << "AJDUST FOR PLATEAU " << p << endl;
		adjust = p;
		if (p) {
			wings_z.setTarget(p->getWidth() / 2);
			pusher_offset_x = p->getX();
		}
	}


}

