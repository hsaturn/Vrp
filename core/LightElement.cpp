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

LightElement::LightElement(float x, float y, float z, float a, bool active)
: mactive(active)
{
	marray[0] = x;
	marray[1] = y;
	marray[2] = z;
	marray[3] = a;
}

string LightElement::read(const string& cmd, string& incoming)
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
		cout << "ONOFF=(" << onoff << ")" << endl;
		marray[0] = StringUtil::getFloat(onoff);
		mactive = true;
		s << "on";
		if (onoff == "on")
			mactive = true;
		else
		{
			int i=1;
			while(i<4 && incoming.length())
				marray[i++] = StringUtil::getFloat(incoming);
		}
	}
	s << " ";
	for(int i=0; i<4; i++)
		s << marray[i] << ' ';
	cout << s.str() << endl;
	return s.str();
}

void LightElement::translate(GLfloat dir)
{
	glTranslatef(marray[0]*dir, marray[1]*dir, marray[2]*dir);
}

void Light::render()
{
	if (mactive)
	{
		static GLUquadricObj *quadric = 0;
		if (quadric==0)
		{
			quadric = gluNewQuadric();
			gluQuadricDrawStyle(quadric, GLU_FILL );
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_POSITION,marray);
		glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,.001f);
		
		Color::yellow.render();
		//glBegin( GL_LINE_LOOP );/// don't workglPointSize( 0.0 );
		translate(1);
		gluSphere( quadric , 1 , 36 , 18 );
		translate(-1);
		
		gluSphere( quadric , 1 , 36 , 18 );
	}
	else
		glDisable(GL_LIGHTING);
	
}
