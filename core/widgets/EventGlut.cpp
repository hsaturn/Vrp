/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EventGlut.hpp"
#include <GL/freeglut_std.h>

#include <iostream>
#include <GL/freeglut_std.h>

using namespace std;

namespace hwidgets
{
	EventGlut* EventGlut::instance = 0;
	bool EventGlut::init = false;

	EventGlut::EventGlut()
	{
		if (init == false)
		{
			cerr << "=================================" << endl;
			cerr << "WARNING :EventGlut not tested yet" << endl;
			cerr << "=================================" << endl;

			init = true ;
			instance = new EventGlut;

			glutKeyboardFunc(KeyboardFunc);
			glutKeyboardUpFunc(KeyboardUpFunc);
			glutMouseFunc(MouseFunc);
			glutMotionFunc(MotionFunc);
			glutPassiveMotionFunc(PassiveMotionFunc);
		}
	}

	EventGlut::~EventGlut()
	{
		if (instance == this)
		{
			instance = 0;
			init = false;
		}
	}
	
	void EventGlut::KeyboardFunc(unsigned char key, int mousex, int mousey)
	{
		if (init==false) return;
		instance->updateModifiers();
		instance->keybd.key = key;
		instance->keybd.state = Keybd::DOWN;
		instance->keybd.mouse_x = mousex;
		instance->keybd.mouse_y = mousey;
	}
	
	void EventGlut::KeyboardUpFunc(unsigned char key, int mousex, int mousey)
	{
		if (init==false) return;
		instance->updateModifiers();
		instance->keybd.key = key;
		instance->keybd.state = Keybd::UP;
		instance->keybd.mouse_x = mousex;
		instance->keybd.mouse_y = mousey;
	}
	
	void EventGlut::MouseFunc(int button, int state, int x, int y)
	{
		if (!init) return;
		instance->updateModifiers();
		instance->mouse.x = x;
		instance->mouse.y = y;
		switch (button)
		{
			case GLUT_LEFT_BUTTON:
				instance->mouse.button = instance->mouse.LEFT;
				break;
			case GLUT_MIDDLE_BUTTON:
				instance->mouse.button = instance->mouse.MIDDLE;
				break;
			case GLUT_RIGHT_BUTTON:
				instance->mouse.button = instance->mouse.RIGHT;
				break;
			default:
				instance->mouse.button = instance->mouse.NONE;
		}
		instance->mouse.state = state;
	}

	void EventGlut::MotionFunc(int x, int y)
	{
		if (!init) return;
		instance->changed = true;
		instance->updateModifiers();
		instance->mouse.x = x;
		instance->mouse.y = y;
	}

	void EventGlut::PassiveMotionFunc(int x, int y)
	{
		if (!init) return;
		instance->changed = true;
		instance->updateModifiers();
		instance->mouse.button = instance->mouse.NONE;
		instance->mouse.x = x;
		instance->mouse.y = y;
	}
	
	void EventGlut::updateModifiers()
	{
		auto get=glutGetModifiers();
		uint16_t amod = keybd.mod;
		keybd.mod = keybd.NONE;
		if (get & GLUT_ACTIVE_CTRL)		keybd.mod |= keybd.CTRL;
		if (get & GLUT_ACTIVE_SHIFT)	keybd.mod |= keybd.SHIFT;
		if (get & GLUT_ACTIVE_ALT)		keybd.mod |= keybd.ALT;
		changed = (keybd.mod != amod);
	}
}