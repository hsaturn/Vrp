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
		glNormal3i(0 ,-1, 0);	// Bottom rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+NAV_LENGTH);
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y, Z1+NAV_LENGTH);
		
		glNormal3i(1 ,0, 0);	// Right rectangle
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
		
		glNormal3i(0, 1, 0);	// Top rectangle
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1+NAV_LENGTH);
			
		glNormal3i(-1, 0, 0);	// Left rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+NAV_LENGTH);
		glEnd();
		
		Color::blue.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,-1);	// back rectangle
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+NAV_HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+NAV_HEIGHT, Z1);
		glEnd();
		
		Color::brown.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,1);	// front rectangle
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