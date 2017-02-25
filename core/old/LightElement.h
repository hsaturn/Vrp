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
#include <DynamicFloat.hpp>
#include <GL/gl.h>

using namespace std;

class DynamicFloat;

class LightElement {
public:
	LightElement(float r, float v, float b, float alpha, bool active);
	
	virtual ~LightElement(){};
	
	// [on|off] r[,v[,b[,a]]]
	virtual string read(string& cmd, string &incoming);
	
	operator bool() const { return mactive; }
	
	
	virtual bool render() { return evolving; }	
	
	bool isReady() const;
	
	// GLfloat marray[4];
	
	DynamicFloat marray[4];
	bool mactive;
	bool evolving;
	
	GLfloat* getFloatArray(DynamicFloat* dyns = 0,uint8_t size=4);
	
	void outDynFloat(ostream&, DynamicFloat*, uint8_t size);
	
};

class Light : public LightElement
{
	public:
		Light(float x, float y, float z, float a, bool active);
		virtual ~Light();
		
		virtual bool render();
		virtual string read(string& cmd, string &incoming);
		void translate(GLfloat dir);
			
	private:
		DynamicFloat position[3];
};

#endif /* LIGHTELEMENT_H */

