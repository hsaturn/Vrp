/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Axis.cpp
 * Author: hsaturn
 * 
 * Created on 21 août 2017, 10:44
 */

#include "Axis.hpp"

Axis::~Axis() { }

void Axis::render()
{
	if (active)
	{
		glBegin(GL_LINES);

		// draw line for x axis
		glColor3f(1.0, 0.0, 0.0); // X ROUGE
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(length, 0.0, 0.0);

		// draw line for y axis
		glColor3f(0.0, 1.0, 0.0); // Y VERT
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, length, 0.0);

		// draw line for Z axis
		glColor3f(0.0, 0.0, 1.0); // Z BLEU
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, length);

		glEnd();
	}
}
