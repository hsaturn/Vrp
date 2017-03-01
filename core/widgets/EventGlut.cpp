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

namespace hwidgets {
	bool EventGlut::init = false;
	map<uint16_t, uint16_t> EventGlut::mapButtons;
	Event::Mouse::Buttons EventGlut::buttons;

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

	void EventGlut::update() { }

	void EventGlut::KeyboardFunc(unsigned char keyc, int mousex, int mousey)
	{
		if (init == false) return;

		EventGlut key;
		key.type = Event::Type::EVT_KEYBD_DOWN;
		key.mod = getModifiers();
		key.key = (Key) keyc;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		events.push(key);
	}

	void EventGlut::KeyboardUpFunc(unsigned char keyc, int mousex, int mousey)
	{
		if (init == false) return;

		EventGlut key;
		key.type = Event::Type::EVT_KEYBD_UP;
		key.mod = getModifiers();
		key.key = (Key) keyc;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		events.push(key);
	}

	void EventGlut::SpecialFunc(int keyc, int mousex, int mousey)
	{
		if (init == false) return;

		EventGlut key;
		key.type = Event::EVT_KEYBD_DOWN;
		key.mod = getModifiers();
		key.key = (Key) keyc;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		events.push(key);
	}

	void EventGlut::MouseFunc(int button, int state, int x, int y)
	{
		if (!init) return;
		
		EventGlut evt;
		
		evt.mod = getModifiers();
		if (state == GLUT_DOWN)
			evt.type = EVT_MOUSE_DOWN;
		else
			evt.type = EVT_MOUSE_UP;
		evt.mouse.x = x;
		evt.mouse.y = y;

		auto it = mapButtons.find(button);
		if (it != mapButtons.end())
		{
			cout << button << "/" << state << endl;
			evt.mouse.button =  it->second;
			if (state == GLUT_DOWN)
			{
				buttons.all |= it->second;
				cout << "DOWN" << endl;
			}
			else
			{
				cout << "[ " << buttons.all << " x=" << it->second << " ";
				buttons.all &= ~it->second;
				cout << " UP " << buttons.all << endl;
			}
			evt.buttons.all = buttons.all;
		}
		else
		{
			evt.mouse.button = Event::Mouse::BTN_NONE;
			cerr << "Glut event error : unknown button " << button << endl;
		}
		events.push(evt);
	}

	void EventGlut::MotionFunc(int x, int y)
	{
		if (!init) return;
		
		EventGlut mouse;
		
		mouse.type = Event::EVT_MOUSE_DOWN;
		mouse.mod = getModifiers();
		mouse.buttons = buttons;
		mouse.mouse.x = x;
		mouse.mouse.y = y;
	}

	void EventGlut::PassiveMotionFunc(int x, int y)
	{
		if (!init) return;
		
		EventGlut mouse;
		
		mouse.type = Event::EVT_MOUSE_MOVE;
		mouse.mod = getModifiers();
		buttons.all = Mouse::BTN_NONE;
		mouse.buttons = buttons;
		mouse.mouse.x = x;
		mouse.mouse.y = y;
		events.push(mouse);
	}

	Event::Modifier EventGlut::getModifiers()
	{
		static Modifier mod = Modifier::NONE;
		static int last = 0;
		int get = glutGetModifiers();
		
		if (get != last)
		{
			EventGlut evt;
			if (last & get)
			{
				cerr << "MOD DOWN" << endl;
				evt.type = Event::EVT_KEYBD_DOWN;
				if (get & GLUT_ACTIVE_CTRL)		mod = (Modifier)(mod & Event::Modifier::CTRL);
				if (get & GLUT_ACTIVE_SHIFT)	mod = (Modifier)(mod & Event::Modifier::SHIFT);
				if (get & GLUT_ACTIVE_ALT)		mod = (Modifier)(mod & Event::Modifier::ALT);
			}
			else
			{
				cerr << "MOD UP" << endl;
				evt.type = Event::EVT_KEYBD_UP;
				int up = get ^ last;
				if (up & GLUT_ACTIVE_CTRL)		mod = (Modifier)(mod & ~Event::Modifier::CTRL);
				if (up & GLUT_ACTIVE_SHIFT)		mod = (Modifier)(mod & ~Event::Modifier::SHIFT);
				if (up & GLUT_ACTIVE_ALT)		mod = (Modifier)(mod & ~Event::Modifier::ALT);
				
			}
			events.push(evt);
			last = get;
		}
		
		return mod;
	}
}