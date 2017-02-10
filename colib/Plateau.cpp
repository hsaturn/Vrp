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

namespace Colib
{
	Plateau::Plateau(string content, int xc)
	{
		cout << "NEW PLATEAU " << content << endl;
		string model = StringUtil::getWord(content);
		pmodel = MyModel::get(model);
		//pmodel = 0;
		if (pmodel == 0)
		{
			cerr << "Model not loaded:" << model << endl;
			content=model+' '+content;
		}
		mcontent = content;
		x.setValue(xc);
		x.setAccel(20);
		x.setMaxValue(9999);
		x.setMinValue(-9999);
		x.setMaxVelocityThreshold(3);
		x.setMaxVelocity(10);
	}
	
	void Plateau::renderAtCenter(int cy, int cz)
	{
		if (pmodel)
		{
			glPushMatrix();
			glTranslatef(x, cy, cz);
			pmodel->render();
			glPopMatrix();
		}
		x.update();
		int x1,x2,y1,y2,z1,z2;
		x1 = x-Column::DEPTH/2;
		x2 = x+Column::DEPTH/2;
		y1 = cy+1;
		y2 = cy+THICKNESS;
		z1 = cz-LENGTH/2;
		z2 = cz+LENGTH/2;
		
		Color::orange.render();
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3i(x1, y1, z1);
		glVertex3i(x2, y1, z1);
		glVertex3i(x1, y1, z2);
		glVertex3i(x2, y1, z2);
		glEnd();
	}
}

