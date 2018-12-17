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
#include <DynamicFloat.hpp>
#include <GL/gl.h>

using namespace std;

class DynamicFloat;

class LightElement {
public:
	LightElement(float x, float y, float z, float a, bool active);
	virtual ~LightElement(){};
		
	string read(string& cmd, string &incoming);
	
	virtual void setValue(int index, float f);
	
	operator bool() const { return mactive; }
	

	virtual bool render() { return false; }	// TODO ??
	
	bool isReady() const;
	
	
	// GLfloat marray[4];
	
	DynamicFloat marray[4];
	bool mactive;
	
	GLfloat* getFloatArray(DynamicFloat* dyns = 0,uint8_t size=4);
	
	void outDynFloat(ostream&, DynamicFloat*, uint8_t size);
	
};

class Light : public LightElement
{
	public:
		Light(float x, float y, float z, float a, bool active);
		
		virtual void setValue(int index, float f);
		virtual bool render();
		void translate(GLfloat dir);
		
	private:
		DynamicFloat* position;
		bool moving;
};

#endif /* LIGHTELEMENT_H */

