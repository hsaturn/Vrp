/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightElement.h
 * Author: francois
 *
 * Created on 27 janvier 2017, 13:37
 */

#ifndef LIGHTELEMENT_H
#define LIGHTELEMENT_H

#include <string>
#include <GL/glew.h>

using namespace std;

class LightElement {
public:
	LightElement(float x, float y, float z, float a, bool active);
		
	string read(const string& cmd, string &incoming);

	operator bool() const { return mactive; }
	
	void translate(GLfloat dir);
	
	GLfloat marray[4];
	bool mactive;
};

class Light : public LightElement
{
	public:
		Light(float x, float y, float z, float a, bool active) : LightElement(x,y,z,a,active) {}
		void render();
};

#endif /* LIGHTELEMENT_H */

