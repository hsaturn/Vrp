#include "Bati.hpp"
#include "Colib.hpp"
#include <GL/glew.h>
#include <Color.h>
#include <StringUtil.hpp>
#include "Navette.hpp"
#include "Cloison.hpp"
#include "Column.hpp"
#include "MotorSpeedHook.hpp"
#include "GLShader.hpp"

namespace Colib {
	
	Bati::Bati(const Colib* p)
	:
	pcolib(p),
	h(0)
	{
		hook_speed = 0;
		navette = new Navette(this);
		h.setMaxVelocity(MAX_SPEED);
		h.setMaxValue(10000);
		h.setMaxVelocityThreshold(10*FACTOR);
		h.setPositionTolerance(0.1);
		h.setAccel(12*FACTOR);
		
		column_dest = 0;
		etage_dest = 0;
		
		changeSound("bati define sound { am 70 100 sinus 200:80 sq 300 sq 600:10 } reverb 30:30 fm 0 80 sound hook");
	}
	
	Bati::~Bati()
	{
		changeSound("");
		delete navette;
	}
	
	bool Bati::isAllStopped()
	{
		if (!h.targetReached())
			return false;
		return navette->isAllStopped();
	}

	void Bati::changeSound(string sound)
	{
		if (sound.length())
		{
			string what = StringUtil::getWord(sound);
			cout << "CHANGING SOUND " << sound << endl;
			if (what == "bati")
			{
				static int number=0;
				string name="hook_bati_"+StringUtil::to_string(number++);
				
				if (hook_speed == 0)
					hook_speed = new MotorSpeedHook(name, MAX_SPEED);
					// delete hook_speed;
				hook_speed->changeSound(sound);
			}
			else if (what == "navette")
				navette->changeSound(sound);
			else
				cerr << "Unknown sound target : " << what  << endl;
		}
		else
		{
			if (hook_speed)
			{
				delete hook_speed;
				hook_speed = 0;
			}
		}
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
		glUseProgram(GLShader::loadGlsl("phong"));
		Color::red.render();
		h.update();
		
		if (hook_speed)
			hook_speed->update(h.getVelocity());

		int x1 = getXLeft();
		const int delta = Bati::THICKNESS;
		pilier(x1, -delta);
		pilier(x1 + Column::DEPTH_X - THICKNESS, -delta);
		pilier(x1, pcolib->getLength() - THICKNESS + delta);
		pilier(x1 + Column::DEPTH_X - THICKNESS, pcolib->getLength() - THICKNESS + delta);
		
		traverses(delta);
		
		bool bRet = navette->render();
		
		bRet |= !h.targetReached();
		
		{
			extern int col;
			extern bool back;
			const int RAY= 2;
			
			static GLUquadricObj *quadric = 0;
			Color::red.render();
			if (quadric==0)
			{
				quadric = gluNewQuadric();
				gluQuadricDrawStyle(quadric, GLU_FILL );
			}
			if (col>=0)
			{
				glPushMatrix();
			
				int x = (back ?  getXLeft() + RAY/2 : getXRight() -RAY/2); // pcolib->getCenterOfColumnX(back) - Column::DEPTH_X/2.0; // Column::DEPTH_X + (back ? RAY : Column::DEPTH_X-RAY);
				glTranslatef(x, h.getTarget()+Navette::HEIGHT_Y+Bati::THICKNESS, pcolib->getCenterOfColumnZ(col, back));
			
				gluSphere( quadric , 2 , 36 , 18 );
				glPopMatrix();
			}
		}
		glUseProgram(0);
		return bRet;
	}
	
	const char* Bati::put(bool back)
	{
		Column* col = pcolib->getColumn(column_dest, back);
		return navette->put(col, etage_dest, back);
	}

	const char* Bati::get(bool back)
	{
		Column* col = pcolib->getColumn(column_dest, back);
		return navette->get(col, etage_dest, back);
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
	
	void Bati::traverses(float delta)
	{
		Color::dark_red.render();
		traverse(getXLeft(), delta);
		traverse(getXRight() - THICKNESS, delta);
	}
	
	bool Bati::isReady()
	{
		if (navette->isReady())
			return true;
		return false;
	}
	
	Object::ExecResult Bati::moveTo(int col_dest, int etage, bool back)
	{
		if (navette->isReady())
		{
			column_dest = col_dest;
			etage_dest = etage;
			cerr << "col_dest=" << col_dest << " / etage_dest=" << etage_dest << endl;

			int total_height = pcolib->getHeight(etage);
			if (navette->getPlateau())
				total_height += navette->getPlateau()->getHeight();
			if (total_height>pcolib->getHeight())
			{
				cerr << "Ca toucherait le plafond !!! " << endl;
				return Object::EXEC_FAILED;
			}
			h.setTarget(pcolib->getHeight(etage) -Navette::HEIGHT_Y-Bati::THICKNESS);
			float z = pcolib->getCenterOfColumnZ(col_dest, back);
			
			if (z==-1)
				return Object::EXEC_FAILED;
			navette->centerOn(z, back);
			return Object::EXEC_OK;
		}
		else
		{
			cout << "Navette not ready" << endl;
			return Object::EXEC_BUSY;
		}
		
		return Object::EXEC_FAILED;
	}
	
	float Bati::getHeight() const
	{
		return h;
	}
	
	float Bati::getTopHeight() const
	{
		return getHeight() + THICKNESS;
	}
	
	float Bati::getLength() const
	{
		return pcolib->getLength();
	}

	float Bati::getXLeft() const
	{
		return Column::DEPTH_X + Cloison::THICKNESS_Z;
	}
	
	float Bati::getXRight() const
	{
		return Column::DEPTH_X*2 + Cloison::THICKNESS_Z;
	}
	
	float Bati::getCenterX() const
	{
		return (getXLeft()+getXRight())/2;
	}
	
	void Bati::traverse(int x, float delta)
	{
		float H = getHeight();
		int Z2 = pcolib->getLength()-THICKNESS+delta;
		glBegin(GL_TRIANGLE_STRIP);
		
		glNormal3i(0, -1, 0);
		glVertex3f(x, H, THICKNESS-delta);
		glVertex3f(x, H, Z2);
		glVertex3f(x+THICKNESS, H, THICKNESS-delta);
		glVertex3f(x+THICKNESS, H, Z2);
		
		glNormal3i(1, 0, 0);
		glVertex3f(x+THICKNESS, H+THICKNESS, THICKNESS-delta);
		glVertex3f(x+THICKNESS, H+THICKNESS, Z2);
		
		glNormal3i(0, 1, 0);
		glVertex3f(x, H+THICKNESS, THICKNESS-delta);
		glVertex3f(x, H+THICKNESS, Z2);
		
		glNormal3i(-1, 0, 0);
		glVertex3f(x, H, THICKNESS-delta);
		glVertex3f(x, H, Z2);
		
		glEnd();

	}
}