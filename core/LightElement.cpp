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
#include <math.h>

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

	cmd = "";
	return s.str();
}

void LightElement::outDynFloat(ostream& s, DynamicFloat* dyns, uint8_t size)
{
	s << (mactive ? "on" : "off") << " Value: (";
	for(int i=0; i<size; i++)
		s << setprecision(2) << (float)dyns[i] << ", ";
	s.seekp(-2,s.cur);

	s << ") Target: (";
	for(int i=0; i<size; i++)
		s << dyns[i].getTarget() << ", ";
	s.seekp(-2,s.cur);

	s << ") Velocity: (";
	for(int i=0; i<size; i++)
		s << dyns[i].getVelocity() << ", ";
	s.seekp(-2,s.cur);

	s << ") Accel: (";
	for(int i=0; i<size; i++)
		s << dyns[i].getVelocity() << ", ";	
	s.seekp(-2,s.cur);
	s << ") ";
}


 Light::Light(float x, float y, float z, float a, bool active)
	:
	LightElement(1,1,1,a,active)
 {
	 rotation_ray = 0;
	 for(int i=0; i<3; i++)
	 {
		position[i].setMinValue(-100);
		position[i].setMaxValue(100);
		position[i].setAccel(1);
		position[i].setMaxVelocityThreshold(20);
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
	bool help=false;
	
	while(incoming.length() && help ==false)
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
		else if (StringUtil::startsWith(incoming, "rotation", true))
		{
			rotation_angle = 0;
			if (StringUtil::startsWithFloat(incoming))
				rotation_ray = StringUtil::getFloat(incoming);
			if (StringUtil::startsWithFloat(incoming))
				rotation_speed = StringUtil::getFloat(incoming);
			else
				help = true;
		}
		else if (StringUtil::startsWith(incoming, "on", true))
			mactive = true;
		else if (StringUtil::startsWith(incoming, "off", true))
			mactive = false;
		else if (StringUtil::startsWith(incoming, "help", true))
			help = true;
		else
			help = true;
	}

	if (help)
	{
		s << "help syntax:" << endl;
		s << "Invalid syntax [rotation {ray} {speed}] (color {color} position [x,y,z]" << endl;
		if (incoming.length())
			s << "Garbage : " << incoming << endl;
	}
	
	s << "position: ";
	outDynFloat(s, position,3);
	if (rotation_ray)
	{
		s << "rotate " << rotation_ray << " " << rotation_speed << "/s";
	}
	cmd="";
	return s.str();
}


bool Light::render()
{
	evolving = false;
		
	if (mactive)
	{
		float* pos = getFloatArray(position, 3);
		if (rotation_ray)
		{
			evolving = true;
			
			std::chrono::time_point<std::chrono::system_clock> cur;
			cur = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsed_seconds = cur-last;
			last = cur;
			
			rotation_angle += 2*M_PI*elapsed_seconds.count()*rotation_speed;
			if (rotation_angle> 2*M_PI)
				rotation_angle -= 2*M_PI;
			
			pos[0] += rotation_ray*cos(rotation_angle);
			pos[2] += rotation_ray*sin(rotation_angle);
		}
		
		static GLUquadricObj *quadric = 0;
		if (quadric==0)
		{
			quadric = gluNewQuadric();
			gluQuadricDrawStyle(quadric, GLU_FILL );
		}

		glDisable(GL_LIGHTING);
		Color::yellow.render();
		//glBegin( GL_LINE_LOOP );/// don't workglPointSize( 0.0 );
		glTranslatef(pos[0], pos[1], pos[2]);
		gluSphere( quadric , 0.2 , 36 , 18 );
		glTranslatef(-pos[0], -pos[1], -pos[2]);
		
		glClearColor (0.0, 0.0, 0.0, 0.0);

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	   glEnable(GL_DEPTH_TEST);
	   
		glLightfv(GL_LIGHT0,GL_POSITION, pos);
		//glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,.01f);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}

	return evolving;
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
	floats[3]=1;
	
	for (int i = 0; i < size; i++)
	{
		if (!dyns[i].targetReached())
			evolving = true;
		dyns[i].update();
		floats[i]=dyns[i];
	}
	return &floats[0];
}	   
