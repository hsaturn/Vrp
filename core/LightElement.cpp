/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightElement.cpp
 * Author: francois
 * 
 * Created on 27 janvier 2017, 13:37
 */

#include "LightElement.h"
#include <sstream>
#include "StringUtil.hpp"
#include "Color.h"
#include "MovingCoord.hpp"

LightElement::LightElement(float x, float y, float z, float a, bool active)
: mactive(active)
{
	marray[0].setTarget(x);
	marray[1].setTarget(y);
	marray[2].setTarget(z);
	marray[3].setTarget(a);

	for(int i=0; i<4; i++)
	{
		marray[i].setAccel(0.3);
		marray[i].setMaxVelocityThreshold(0.3);
		marray[i].setMaxVelocity(0.3);
		marray[i].setMinValue(0);
		marray[i].setMaxValue(1);
		marray[i].setPositionTolerance(0.01);
	}
	
}

bool LightElement::isReady() const
{
	for(int i=0; i<4; i++)
		if (!marray[i].targetReached())
			return false;
	return true;
}

string LightElement::read(string& cmd, string& incoming)
{
	stringstream s;
	s << cmd << " : ";

	string onoff = StringUtil::getWord(incoming);
	if (onoff == "off")
	{
		mactive = false;
		s << "off";
	}
	else if (onoff == "on")
	{
		mactive = true;
		s << "on";
	}
	else if (onoff.length())
	{
		float value = StringUtil::getFloat(onoff);
		setValue(0, value);
		mactive = true;
		s << "on";
		if (onoff == "on")
			mactive = true;
		else
		{
			int i=1;
			while(i<3)
			{
				if (incoming.length())
					value = StringUtil::getFloat(incoming);
				setValue(i++, value);
			}
			if (incoming.length())
				setValue(4, StringUtil::getFloat(incoming));
			
		}
	}
	s << " ";
	for(int i=0; i<4; i++)
		s << marray[i] << ' ';
	cout << s.str() << endl;
	cmd = "";
	return s.str();
}

void LightElement::translate(GLfloat dir)
{
	glTranslatef(marray[0]*dir, marray[1]*dir, marray[2]*dir);
}

 Light::Light(float x, float y, float z, float a, bool active)
	:
	LightElement(x,y,z,a,active)
 {
	 dest = new MovingCoord[4];
	 for(int i=0; i<4; i++)
	 {
		dest[i].setMinValue(-100);
		dest[i].setMaxValue(100);
		dest[i].setAccel(1);
		dest[i].setMaxVelocityThreshold(5);
		dest[i]=marray[i];
	 }
 }
 
 Light::~Light()
 {
	 delete [] dest;
 }

bool Light::render()
{
	moving = false;
	for(int i=0; i<4; i++)
	{
		dest[i].update();
		if (!dest[i].targetReached())
			moving = true;
		// marray[i] = dest[i];
	}
		
	if (mactive)
	{
		static GLUquadricObj *quadric = 0;
		glClearColor (0.0, 0.0, 0.0, 0.0);
		if (quadric==0)
		{
			quadric = gluNewQuadric();
			gluQuadricDrawStyle(quadric, GLU_FILL );
		}

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	   glEnable(GL_DEPTH_TEST);
	   
		glLightfv(GL_LIGHT0,GL_POSITION, getFloatArray());
		//glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,.01f);
		
		Color::yellow.render();
		//glBegin( GL_LINE_LOOP );/// don't workglPointSize( 0.0 );
		translate(1);
		gluSphere( quadric , 0.2 , 36 , 18 );
		translate(-1);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}

	return moving;
}

GLfloat*  LightElement::getFloatArray()
{
	static GLfloat farray[4];
	for (int i = 0; i < 4; i++)
	{
		marray[i].update();
		farray[i] = marray[i];
	}
	return &farray[0];
}	   
	   


void Light::setValue(int index, float f)
{
	moving = true;
	dest[index].setTarget(f);
	marray[index].setTarget(dest[index]);
}

void LightElement::setValue(int index, float f)
{
	marray[index].setTarget(f);
}