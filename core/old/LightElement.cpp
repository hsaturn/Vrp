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

	if (StringUtil::startsWith(incoming, "off", true))
		mactive = false;
	else if (StringUtil::startsWith(incoming, "on", true))
		mactive = true;
	
	if (StringUtil::startsWithFloat(incoming))
	{
		mactive = true;
		float value = StringUtil::getFloat(incoming);
		int i=0;
		while(i<3)
		{
			if (StringUtil::startsWithFloat(incoming))
				value = StringUtil::getFloat(incoming);
			marray[i++].setTarget(value);
		}
		if (StringUtil::startsWithFloat(incoming))
			marray[3].setValue(StringUtil::getFloat(incoming));
	}
	else
		cerr << "bad syntax, [on|off] float[,float...] expected." << endl;

	outDynFloat(s, marray, 4);

	s << ")";
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
 
 Light::~Light()
 {
 }
 
string Light::read(string& cmd, string &incoming)
{
	stringstream s;
	s << cmd << ' ';
	
	while(incoming.length())
	{
		if (StringUtil::startsWith(incoming, "color ", true))
		{
			s << LightElement::read(cmd, incoming);
		}
		else if (
			StringUtil::startsWith(incoming, "pos ", true) ||
			StringUtil::startsWith(incoming, "position ", true))
		{
			int i=0;
			while(StringUtil::startsWithFloat(incoming) && i<3)
				position[i++].setTarget(StringUtil::getFloat(incoming));
		}
		else if (StringUtil::startsWith(incoming, "on", true))
			mactive = true;
		else if (StringUtil::startsWith(incoming, "off", true))
			mactive = false;
		else
		{
			s << "Invalid syntax (color [r,g,b,a] position [x,y,z]" << endl;
			s << "incoming: " << incoming << endl;
			incoming="";
		}
	}
	s << "position: ";
	outDynFloat(s, position,3);
	cmd="";
	return s.str();
}


bool Light::render()
{
	evolving = false;
		
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
	   
		glLightfv(GL_LIGHT0,GL_POSITION, getFloatArray(position,3));
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

	return !isReady();
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
		if (dyns[i].targetReached())
			evolving = true;
		dyns[i].update();
		floats[i]=dyns[i];
	}
	return &floats[0];
}	   
