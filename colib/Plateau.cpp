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
#include <libsynth.hpp>
#include "MotorSpeedHook.hpp"
#include "Colib.hpp"

namespace Colib
{
	Plateau* Plateau::factory(string content, int xc)
	{
		Plateau* p = new Plateau(content,xc);
		if (p->getWidth()==0)
		{
			delete p;
			p = 0;
		}
		else
			cout << "PLATEAU WIDTH : " << p->getWidth() << endl;
		return p;
	}
	
	Plateau::~Plateau()
	{
		if (speed_hook) delete speed_hook;
	}
	
	Plateau::Plateau(string content, int xc)
	{
		mcontent = content;
		cout << "NEW PLATEAU " << content << endl;
		string model = StringUtil::getWord(content);
		pmodel = Model::get(model);
		//pmodel = 0;
		if (pmodel == 0)
		{
			cerr << "Model not loaded:" << model << endl;
			content=model+' '+content;
		}
		
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
			
			
			offset_model[0] = (-hig[0]-low[0])/2;
			offset_model[1] = -low[1];
			offset_model[2] = (-hig[2]-low[2])/2;
			
			width_z = hig[2]-low[2];
			height_y = hig[1]-low[1];
			length_x = hig[0]-low[0];
			
			if (length_x > 60)
			{
				cerr << "Model too large to enter colib" << endl;
				width_z = 0;
			}
			else if (width_z > 60)
			{
				cerr << "Model too large to enter colib" << endl;
				width_z = 0;
			}
			else if (width_z < 30)
				width_z = 30;
			else if (width_z < 60)
				width_z = 60;
			else
				width_z = 0;	// Model too large
			
			plateau = 0;
			if (width_z)
			{
				string name="plateau_"+StringUtil::to_string(width_z);
				plateau = Model::get(name);
				
				if (plateau)
				{
					auto low = plateau->getMinCoord();
					auto hig = plateau->getMaxCoord();

					plateau_offset[0] = (-hig[0]-low[0])/2;
					plateau_offset[1] = -low[1];
					plateau_offset[2] = (-hig[2]-low[2])/2;
				}
			}
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
		x.update();
		if (pmodel)
		{
			glPushMatrix();
			
			glTranslatef(x+offset_model[0], cy+offset_model[1]+0.1, cz+offset_model[2]);
			
			pmodel->render();
			if (Colib::renderBoundingBoxes())
			{
				Color::red.render();
				pmodel->renderBoundingBox();
			}
			glPopMatrix();
		}
		speed_hook->update(x.getVelocity());

		string name="plateau_"+StringUtil::to_string(width_z);
		
		if (plateau)
		{
			glPushMatrix();
			glTranslatef(x+plateau_offset[0], cy+plateau_offset[1]+0.01, cz+plateau_offset[2]);
			plateau->render();
			glPopMatrix();
			Color::orange.render();
		}
		else
		{
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
	
}

