/* 
 * File:   Plateau.cpp
 * Author: francois
 * 
 * Created on 2 février 2017, 11:58
 */
#include "Plateau.hpp"
#include "Column.hpp"
#include "Color.h"
#include "StringUtil.hpp"
#include <GL/glew.h>
#include <ModelOld.hpp>
#include <model/Model.hpp>
#include "MotorSpeedHook.hpp"

namespace Colib
{
	Plateau* Plateau::factory(string content, int xc)
	{
		Plateau* p = new Plateau(content,xc);
		if (p->length_x > 60)
		{
			cerr << "Model too large to enter colib" << endl;
			p->width_z = 0;
		}
		else if (p->width_z > 60)
		{
			cerr << "Model too large to enter colib" << endl;
			p->width_z = 0;
		}
		else if (p->width_z > 30)
			p->width_z = 60;
		else if (p->width_z)
			p->width_z = 30;
		if (p->width_z == 0)
		{
			delete p;
			p = 0;
		}
		cout << "PLATEAU WIDTH : " << p->getWidth() << endl;
		return p;
	}
	
	Plateau::~Plateau()
	{
		delete speed_hook;
	}
	
	Plateau::Plateau(string content, int xc)
	{
		cout << "NEW PLATEAU " << content << endl;
		string model = StringUtil::getWord(content);
		pmodel = Model::get(model);
		//pmodel = 0;
		if (pmodel == 0)
		{
			cerr << "Model not loaded:" << model << endl;
			content=model+' '+content;
		}
		mcontent = content;
		
		const int MAX_SPEED = 20;
		x.setValue(xc);
		x.setAccel(20);
		x.setMaxValue(9999);
		x.setMinValue(-9999);
		x.setMaxVelocityThreshold(MAX_SPEED);
		x.setMaxVelocity(MAX_SPEED);
		
		width_z = 0;
		if (pmodel)
		{
			auto low = pmodel->getMinCoord();
			auto hig = pmodel->getMaxCoord();
			
			width_z = hig[2]-low[2];
			height_y = hig[1]-low[1];
			length_x = hig[0]-low[0];
			cout << "WIDTH (" << model << ")=" << width_z << endl;
		}
		
		static int number=0;
		string name = "plathook_"+StringUtil::to_string(number++);
		speed_hook = new MotorSpeedHook(name, MAX_SPEED);
		cout << "BUILDING SOUND GENERATOR" << endl;
		
		string s;
		s = "reverb 30:30 fm 0 100 am 0 100 square 30:30 triangle 100 hook";
		s = "fm 50 150 fm 80 100  triangle 100 sin 30 hook";
		s = "define sound { am 0 100 { sq 300:10 sq 302:10 } sq 100 } fm 0 80 sound hook";
		
		speed_hook->changeSound(s);		
	}
	
	void Plateau::renderAtCenter(float cy, float cz)
	{
		if (pmodel)
		{
			glPushMatrix();
			auto low = pmodel->getMinCoord();
			auto hig = pmodel->getMaxCoord();
			low[0] = -low[0]-hig[0]/2;
			low[1] = -low[1];
			low[2] = -low[2]-hig[2]/2;
			glTranslatef(x+low[0], cy+low[1]+0.1, cz+low[2]);
			pmodel->render();
			glPopMatrix();
		}
		x.update();
		speed_hook->update(x.getVelocity());
		float x1,x2,y1,y2,z1,z2;
		x1 = x-Column::DEPTH_X/2;
		x2 = x+Column::DEPTH_X/2;
		y1 = cy+0.01;
		y2 = cy+THICKNESS;
		z1 = cz-width_z/2;
		z2 = cz+width_z/2;
		
		Color::orange.render();
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1);
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glEnd();
	}
}

