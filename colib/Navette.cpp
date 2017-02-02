#include <GL/glew.h>

#include "Navette.hpp"
#include "Bati.hpp"
#include "Plateau.hpp"

namespace Colib
{
	Navette::Navette(Bati* bati) : pbati(bati), z(0), plateau(0) 
	{
#define FACTOR 4
		z.setTarget(0);
		z.setMaxVelocity(20*FACTOR);
		z.setMaxValue(100000);
		z.setMaxVelocityThreshold(20*FACTOR);
		z.setAccel(10*FACTOR);
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
		
		int Z1 = z;
		if (z < Bati::THICKNESS)
			z = Bati::THICKNESS;
		float Y = pbati->getTopHeight();
		
		Color::cyan.render();
		
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0 ,-1, 0);	// Bottom rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH);
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y, Z1+LENGTH);
		
		glNormal3i(1 ,0, 0);	// Right rectangle
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT, Z1+LENGTH);
		
		glNormal3i(0, 1, 0);	// Top rectangle
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT, Z1+LENGTH);
			
		glNormal3i(-1, 0, 0);	// Left rectangle
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH);
		glEnd();
		
		Color::blue.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,-1);	// back rectangle
		glVertex3f(pbati->getXRight()+debord, Y, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT, Z1);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT, Z1);
		glEnd();
		
		Color::brown.render();
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3i(0,0,1);	// front rectangle
		glVertex3f(pbati->getXRight()+debord, Y, Z1+LENGTH);
		glVertex3f(pbati->getXLeft()-debord, Y, Z1+LENGTH);
		glVertex3f(pbati->getXRight()+debord, Y+HEIGHT, Z1+LENGTH);
		glVertex3f(pbati->getXLeft()-debord, Y+HEIGHT, Z1+LENGTH);
		glEnd();
		
		if (plateau)
			plateau->renderAtCenter(Y+HEIGHT, z);
		
		return !z.targetReached();
	}
	
	void Navette::centerOn(int zz)
	{
		z.setTarget(zz-Navette::LENGTH/2);
	}
	
	bool Navette::isReady() const
	{
		if (plateau)
			return plateau->isReady();
		else
			return true;
	}
}