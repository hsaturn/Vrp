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
#include <MovingCoord.hpp>
#include <GL/gl.h>

using namespace std;

class MovingCoord;

class LightElement {
public:
	LightElement(float x, float y, float z, float a, bool active);
	virtual ~LightElement(){};
		
	string read(string& cmd, string &incoming);
	
	virtual void setValue(int index, float f);
	
	operator bool() const { return mactive; }
	
	void translate(GLfloat dir);
	
	virtual bool render() { return false; }	// TODO ??
	
	bool isReady() const;
	
	
	// GLfloat marray[4];
	
	MovingCoord marray[4];
	bool mactive;
	
	GLfloat*	getFloatArray();
	
};

class Light : public LightElement
{
	public:
		Light(float x, float y, float z, float a, bool active);
		virtual ~Light();
		
		virtual void setValue(int index, float f);
		virtual bool render();
		
	private:
		MovingCoord* dest;
		bool moving;
};

#endif /* LIGHTELEMENT_H */

