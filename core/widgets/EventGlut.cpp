/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EventGlut.hpp"
#include <GL/freeglut_std.h>

#include <iostream>
#include <iomanip>

#include <GL/freeglut_std.h>

using namespace std;

namespace hwidgets
{
	bool EventGlut::init = false;
	map<uint16_t, uint16_t> EventGlut::mapButtons;
	map<uint16_t, uint16_t> EventGlut::mapSpecial;
	Event::Mouse::Buttons EventGlut::curr_buttons;
	

	EventGlut::EventGlut() { }

	Event* EventGlut::getInstance()
	{
		if (init == false && instance == 0)
		{
			readEvt("glut_buttons", mapButtons);
			Event::readMap("special.kmap", mapSpecial);

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

		static Event key;
		key.type = Event::Type::EVT_KEYBD_DOWN;
		key.mod = getModifiers();
		key.key = (Key) keyc;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		processEvent(key);
	}

	void EventGlut::KeyboardUpFunc(unsigned char keyc, int mousex, int mousey)
	{
		if (init == false) return;

		static Event key;
		key.type = Event::Type::EVT_KEYBD_UP;
		key.mod = getModifiers();
		key.key = (Key) keyc;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		processEvent(key);
	}

	void EventGlut::SpecialFunc(int keyc, int mousex, int mousey)
	{
		if (init == false) return;
		auto it = mapSpecial.find(keyc);
		if (it == mapSpecial.end())
		{
			cerr << "EventGlut:warning ignoring special key #" << (uint16_t)keyc << " 0x" << hex << (uint16_t)keyc << ", modify special.kmap to fix that." << endl;
			cerr << dec;
			return;
		}
		static Event key;
		key.type = Event::EVT_KEYBD_DOWN;
		key.mod = getModifiers();
		key.key = it->second;
		key.mouse.x = mousex;
		key.mouse.y = mousey;
		processEvent(key);
	}

	void EventGlut::MouseFunc(int button, int state, int x, int y)
	{
		if (!init) return;

		static Event evt;

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
			cout << button << "/" << state << " map=" << it->second << endl;
			evt.mouse.button =  it->second;
			if (state == GLUT_DOWN)
			{
				curr_buttons.all |= it->second;
				cout << "DOWN" << endl;
			}
			else
			{
				cout << "[ " << curr_buttons.all << " x=" << it->second << " ";
				curr_buttons.all &= ~it->second;
				cout << " UP " << curr_buttons.all << " o" << oct << (~it->second) << dec << endl;
			}
			evt.mouse.buttons = curr_buttons;
			cout << evt << endl;
		}
		else
		{
			evt.mouse.button = Event::Mouse::BTN_NONE;
			cerr << "Glut event error : unknown button " << button << endl;
		}
		processEvent(evt);
	}

	void EventGlut::MotionFunc(int x, int y)
	{
		if (!init) return;

		static Event evt;

		evt.type = Event::EVT_MOUSE_MOVE;
		evt.mod = getModifiers();
		evt.mouse.buttons = curr_buttons;
		evt.mouse.x = x;
		evt.mouse.y = y;
		processEvent(evt);
	}

	void EventGlut::PassiveMotionFunc(int x, int y)
	{
		if (!init) return;

		static Event evt;

		evt.type = Event::EVT_MOUSE_MOVE;
		evt.mod = getModifiers();
		curr_buttons.all = Mouse::BTN_NONE;
		evt.mouse.buttons = curr_buttons;
		evt.mouse.x = x;
		evt.mouse.y = y;
		processEvent(evt);
	}

	Event::Modifier EventGlut::getModifiers()
	{
		static Modifier mod = Modifier::NONE;
		static int last = 0;
		int get = glutGetModifiers();

		if (get != last)
		{
			static Event evt;
			int up;
			if (last & get)
			{
				cerr << "MOD DOWN" << endl;
				evt.type = Event::EVT_KEYBD_DOWN;
				up = get;
				if (get & GLUT_ACTIVE_CTRL)		mod = (Modifier) (mod & Event::Modifier::CTRL);
				if (get & GLUT_ACTIVE_SHIFT)	mod = (Modifier) (mod & Event::Modifier::SHIFT);
				if (get & GLUT_ACTIVE_ALT)		mod = (Modifier) (mod & Event::Modifier::ALT);
			}
			else
			{
				cerr << "MOD UP" << endl;
				evt.type = Event::EVT_KEYBD_UP;
				up = get ^ last;
				if (up & GLUT_ACTIVE_CTRL)		mod = (Modifier) (mod & ~Event::Modifier::CTRL);
				if (up & GLUT_ACTIVE_SHIFT)		mod = (Modifier) (mod & ~Event::Modifier::SHIFT);
				if (up & GLUT_ACTIVE_ALT)		mod = (Modifier) (mod & ~Event::Modifier::ALT);

			}
			if (up & GLUT_ACTIVE_CTRL)			evt.key = KEY_CTRL;
			else if (up & GLUT_ACTIVE_SHIFT)	evt.key = KEY_SHIFT;
			else if (up & GLUT_ACTIVE_ALT)		evt.key = KEY_ALT;

			//processEvent(evt);
			last = get;
		}

		return mod;
	}
}
