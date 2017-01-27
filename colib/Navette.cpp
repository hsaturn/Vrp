#include <GL/glew.h>

#include "Navette.hpp"
#include "Bati.hpp"

namespace Colib
{
	Navette::Navette(Bati* bati) : pbati(bati), z(0) 
	{
		z.setTarget(100);
		z.setMaxVelocity(20);
		z.setMaxValue(100);
		z.setMaxVelocityThreshold(20);
		z.setAccel(10);
	}
	
	bool Navette::render()
	{
		const int debord = 1;
		const int dz = Bati::BATI_THICK;
		z.update();
		
		int wu = pbati->getLength()-NAV_LENGTH-dz*2;	// longueur Z utile
		if (wu < 0)
		{
			cout << "BAD WU" << endl;
			
			return false;
		}
		
		int Z1 = (float)wu * z / 100 +dz;
		float Y = pbati->getTopHeight();
		
		Color::cyan.render();
		glBegin(GL_TRIANGLE_STRIP);
		
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+NAV_LENGTH);
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y, Z1+NAV_LENGTH);
		
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
		
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
			
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+NAV_LENGTH);

		Color::blue.render();
		glEnd();
		
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1);
		glEnd();
		
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(pbati->getXRight()+debord, Y, Z1+NAV_LENGTH);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+NAV_LENGTH);
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
		glEnd();
		
		return !z.targetReached();
	}
	
	void Navette::moveTo(int z_percent)
	{
		z.setTarget(z_percent);
	}
}