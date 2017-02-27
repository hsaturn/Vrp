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
	bool EventGlut::init = false;
	map<uint16_t, uint16_t> EventGlut::mapButtons;

	EventGlut::EventGlut() { }

	Event* EventGlut::getInstance()
	{
		if (init == false && instance == 0)
		{
			readEvt("glut_buttons", mapButtons);
			Event::readKeymap("glut_keys.kmap");

			init = true ;
			instance = new EventGlut;

			glutKeyboardFunc(KeyboardFunc);
			glutKeyboardUpFunc(KeyboardUpFunc);
			glutMouseFunc(MouseFunc);
			glutMotionFunc(MotionFunc);
			glutPassiveMotionFunc(PassiveMotionFunc);
			glutSpecialFunc(SpecialFunc);
		}
		return instance;
	}

	EventGlut::~EventGlut()
	{
		if (instance == this)
		{
			instance = 0;
			init = false;
		}
	}

	void EventGlut::_poll()
	{
		event.all = current_event.all;
		current_event.all = 0;
	}

	void EventGlut::KeyboardFunc(unsigned char key, int mousex, int mousey)
	{
		if (init == false) return;
		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		glut->current_event.type.key_down = true;
		glut->updateModifiers();
		glut->keybd.key = key;
		glut->keybd.state = Keybd::DOWN;
		glut->keybd.mouse_x = mousex;
		glut->keybd.mouse_y = mousey;
	}

	void EventGlut::KeyboardUpFunc(unsigned char key, int mousex, int mousey)
	{
		if (init == false) return;
		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		glut->current_event.type.key_up = true;
		glut->updateModifiers();
		glut->keybd.key = key;
		glut->keybd.state = Keybd::UP;
		glut->keybd.mouse_x = mousex;
		glut->keybd.mouse_y = mousey;
	}

	void EventGlut::SpecialFunc(int key, int mousex, int mousey)
	{
		if (init == false) return;
		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		glut->current_event.type.key_down = true;
		glut->updateModifiers();
		glut->keybd.mouse_x = mousex;
		glut->keybd.mouse_y = mousey;
	}

	void EventGlut::MouseFunc(int button, int state, int x, int y)
	{
		if (!init) return;

		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		Event::Mouse &mouse = glut->mouse;

		glut->current_event.type.mouse_button = true;
		glut->updateModifiers();
		mouse.x = x;
		mouse.y = y;
		
		auto it = mapButtons.find(button);
		if (it != mapButtons.end())
		{
			cout << button << "/" << state << endl;
			uint16_t last_button = it->second;
			mouse.button_down = state==GLUT_DOWN;
			
			mouse.last_button = last_button;
			if (state == GLUT_DOWN)
				mouse.buttons.all |= last_button;
			else
				mouse.buttons.all &= ~last_button;
		}
		else
		{
				mouse.last_button = Event::Mouse::NONE;
				cerr << "Glut event error : unknown button " << button << endl;
		}
	}

	void EventGlut::MotionFunc(int x, int y)
	{
		if (!init) return;
		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		glut->current_event.type.mouse_move = true;
		glut->updateModifiers();
		glut->mouse.x = x;
		glut->mouse.y = y;
	}

	void EventGlut::PassiveMotionFunc(int x, int y)
	{
		if (!init) return;
		EventGlut* glut = dynamic_cast<EventGlut*> (instance);
		glut->current_event.type.mouse_move = true;
		glut->updateModifiers();
		glut->mouse.buttons.all = 0;
		glut->mouse.x = x;
		glut->mouse.y = y;
	}

	void EventGlut::updateModifiers()
	{
		auto get = glutGetModifiers();
		uint16_t amod = keybd.mod;
		keybd.mod = keybd.NONE;
		if (get & GLUT_ACTIVE_CTRL)		keybd.mod |= keybd.CTRL;
		if (get & GLUT_ACTIVE_SHIFT)	keybd.mod |= keybd.SHIFT;
		if (get & GLUT_ACTIVE_ALT)		keybd.mod |= keybd.ALT;

		// @FIXME -> should detect key up / down
		if (keybd.mod != amod)
			event.type.key_down = true;
	}
}