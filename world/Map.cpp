/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: francois
 * 
 * Created on 3 janvier 2017, 13:20
 */
#include <GL/glew.h>
#include <Color.h>
#include "Map.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


namespace World
{
	#define H(x,y) (heights[(x)+(y)*width])
	

	Map::Map(int w, int l, float hscale) 
		: width(w), length(l)
	{
		heights = new uint8_t[width*length];
		resetMap();

		for(int y=4; y<=6; y++)
			for(int x=4; x<=6; x++)
				H(x,y)=1;
		H(5,5)=2;
		
		scale_h = hscale;
		cout << "Map scale : " << scale_h << endl;
	}
	
	void Map::resetMap()
	{
		for(int x=0; x<width; x++)
			for(int y=0; y<length; y++)
				H(x,y)=0;
	}

	bool Map::readFile(string incoming)
	{
		bool ret = false;
		int row(0);
		cout << "Reading map " << incoming << endl;
		ifstream f(incoming);
		while(f.good())
		{
			row++;
			int x,y;
			float z;
			f >> x;
			f >> y;
			f >> z;
			if (x>=0 && x<width && y>=0 && y<length)
				H(x,length-y)=z;
			else
				cerr << "Invalid coord : " << x << ',' << y << " at row " << row << endl;
		}
		return ret;
	}
	
	Map::~Map()
	{
		delete[] heights;
	}
	
	bool Map::render()
	{
		const float delta=0.0001f;
		float dx = 1.0f/width;
		float dz = 1.0f/length;
		float fz = -(length/2.0f)*dz;
		float dy = scale_h*(float)(dx+dz)/2; //1.0f/20.0f;
		dy = 0.002;
		//dy +=0.1;
		Color::red.render();
		const Color* color=0;
				
		for(int z=0; z<length-1; z++)
		{
			float fx = -(width/2)*dx;
			for(int x=0; x<width-1; x++)
			{
				auto h = H(x,z);
				float h1=H(x,z) * dy;
				float h2=H(x+1,z) * dy;
				float h3=H(x,z+1) * dy;
				float h4=H(x+1,z+1) * dy;
				
				if (h1<=0 && h2<=0 && h3<=0 && h4<=0)
					color = &Color::cyan;
				else if (h>45)
					color = &Color::white;
				else if (h>15)
					color = &Color::brown;
				else
					color = &Color::green;
				
				if (z==0 && x>7)
				{
					Color::dark_cyan.render();
				}
				
				bool lines = !((h1==h2 && h3==h4)||(h1==h3 && h2==h4));
				
				if (h3>h1 || h3>h4 || h2>h1 || h2>h4)
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(fx+dx, h2, fz);
					glVertex3f(fx, h1 ,fz);
					glVertex3f(fx+dx, h4, fz+dz);
					glVertex3f(fx, h3, fz+dz);
					glEnd();
					Color::black.render();
					if (lines)
					{
						glBegin(GL_LINE_LOOP);
						glVertex3f(fx+dx, h2+delta, fz);
						glVertex3f(fx,	  h1 +delta,fz);
						glVertex3f(fx+dx, h4+delta, fz+dz);
						glEnd();
						glBegin(GL_LINE_LOOP);
						glVertex3f(fx,		h1+delta ,fz);
						glVertex3f(fx+dx,	h4+delta, fz+dz);
						glVertex3f(fx,		h3+delta, fz+dz);
						glEnd();
					}
				}
				else
				{
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(fx, h1 ,fz);
					glVertex3f(fx+dx, h2, fz);
					glVertex3f(fx, h3, fz+dz);
					glVertex3f(fx+dx, h4, fz+dz);
					glEnd();
					Color::black.render();
					if (lines)
					{
						glBegin(GL_LINE_LOOP);
						glVertex3f(fx,		h1+delta ,fz);
						glVertex3f(fx+dx,	h2+delta, fz);
						glVertex3f(fx,		h3+delta, fz+dz);
						glEnd();
						glBegin(GL_LINE_LOOP);
						glVertex3f(fx+dx,	h2+delta, fz);
						glVertex3f(fx,		h3+delta, fz+dz);
						glVertex3f(fx+dx,	h4+delta, fz+dz);
						glEnd();
					}
				}
				
				if (!lines)
				{
					Color::black.render();
					glBegin(GL_LINE_LOOP);
					glVertex3f(fx,		h1 +delta,fz);
					glVertex3f(fx+dx,	h2+delta, fz);
					glVertex3f(fx+dx,	h4+delta, fz+dz);
					glVertex3f(fx,		h3+delta, fz+dz);
					glEnd();
				}
				fx += dx;
				color->render();
			}
			fz += dz;
		}
		return false;
	}
}