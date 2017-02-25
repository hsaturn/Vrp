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
#include "DynamicFloat.hpp"
#include <iomanip>

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
	outDynFloat(s, marray, 4);
	cmd = "";
	return s.str();
}

void LightElement::outDynFloat(ostream& s, DynamicFloat* dyns, uint8_t size)
{
	s << (mactive ? "on" : "off") << " Value: (";
	for(int i=0; i<size; i++)
		s << setprecision(2) << (float)dyns[i] << ", ";

	s << ") Target: (";
	for(int i=0; i<size; i++)
		s << dyns[i].getTarget() << ", ";

	s << ") Velocity: (";
	for(int i=0; i<size; i++)
		s << dyns[i].getVelocity() << ", ";

	s << ") Accel: ";
	for(int i=0; i<size; i++)
		s << dyns[i].getVelocity() << ", ";	
}

void Light::translate(GLfloat dir)
{
	glTranslatef(position[0]*dir, position[1]*dir, position[2]*dir);
}

 Light::Light(float x, float y, float z, float a, bool active)
	:
	LightElement(1,1,1,a,active)
 {
	 position = new DynamicFloat[4];
	 for(int i=0; i<3; i++)
	 {
		position[i].setMinValue(-100);
		position[i].setMaxValue(100);
		position[i].setAccel(1);
		position[i].setMaxVelocityThreshold(5);
	 }
	 position[0].setTarget(x);
	 position[1].setTarget(y);
	 position[2].setTarget(z);
 }
 
bool Light::render()
{
	moving = false;
	for(int i=0; i<3; i++)
	{
		position[i].update();
		if (!position[i].targetReached())
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

GLfloat*  LightElement::getFloatArray(DynamicFloat* dyns,uint8_t size)
{
	if (dyns==0)
		dyns=marray;
	if (size>4)
	{
		size=4;
		cerr << "ERROR: Size limited to 4 in LightElement::getFloatArray()" << endl;
	}
	static GLfloat floats[4];
	floats[4]=1;
	   
	for (int i = 0; i < size; i++)
	{
		//	if (dyns[i].targetReached())
		//		evolving = true;
			dyns[i].update();
			floats[i]=dyns[i];
	}

	return &floats[0];
}

void Light::setValue(int index, float f)
{
	if (index>3) return;
	moving = true;
	position[index].setTarget(f);
	marray[index].setTarget(position[index]);
}

void LightElement::setValue(int index, float f)
{
	marray[index].setTarget(f);
}