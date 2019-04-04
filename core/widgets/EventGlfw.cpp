/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EventGlfw.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

namespace hwidgets
{
	bool EventGlfw::init = false;
	map<uint16_t, uint16_t> EventGlfw::mapButtons;
	map<uint16_t, uint16_t> EventGlfw::mapKeys;
	Event::Mouse::Buttons EventGlfw::curr_buttons;
	int EventGlfw::last=0;
	Event* EventGlfw::key_evt=nullptr;
	

	EventGlfw::EventGlfw() { }

	Event* EventGlfw::getInstance(GLFWwindow *window)
	{
		if (init == false && instance == 0)
		{
			readEvt("glfw_buttons", mapButtons);
			Event::readMap("glfw.kmap", mapKeys);

			init = true ;
			instance = new EventGlfw;

			glfwSetKeyCallback(window, KeyboardFunc);
			glfwSetMouseButtonCallback(window, MouseFunc);
			glfwSetCursorPosCallback(window, MotionFunc);
			glfwSetScrollCallback(window, MotionScroll);
			glfwSetCharCallback(window, CharCallback);
		}
		return instance;
	}

	EventGlfw::~EventGlfw()
	{
		if (instance == this)
		{
			instance = 0;
			init = false;
		}
	}

	void EventGlfw::update() { }

	void EventGlfw::CharCallback(GLFWwindow* window, unsigned int c)
	{
		if (key_evt)
		{
			key_evt->key = c;
			processEvent(*key_evt);
			delete key_evt;
		}
	}

	void EventGlfw::KeyboardFunc(GLFWwindow* window, int keyc, int scan, int action, int mods)
	{
		if (init == false) return;

		key_evt = new Event;
		if (action == GLFW_PRESS)
			key_evt->type = Event::Type::EVT_KEYBD_DOWN;
		else
			key_evt->type = Event::Type::EVT_KEYBD_UP;
		key_evt->mod = getModifiers(mods);
		double x,y;
		glfwGetCursorPos(window, &x, &y);
		key_evt->mouse.x = x;
		key_evt->mouse.y = y;
		auto it=mapKeys.find(keyc);
		if (it != mapKeys.end())
		{
			key_evt->key = (Key) it->second;
			processEvent(*key_evt);
			delete key_evt;
		}
		else
		{
			// Workaround here, the event will be processed later in CharCallBack
			// processEvent(*key_evt);
		}
	}

	void EventGlfw::MouseFunc(GLFWwindow* window, int button, int action, int mods)
	{
		cout << "MOUSE " << button << ", " << action << ", mods=" << mods << endl;
		if (!init) return;

		static Event evt;

		evt.mod = getModifiers(mods);
		if (action == GLFW_PRESS)
			evt.type = EVT_MOUSE_DOWN;
		else
			evt.type = EVT_MOUSE_UP;

		double x,y;
		glfwGetCursorPos(window, &x, &y);
		evt.mouse.x = x;
		evt.mouse.y = y;

		auto it = mapButtons.find(button);
		if (it != mapButtons.end())
		{
			cout << button << "/" << action << " map=" << it->second << endl;
			evt.mouse.button =  it->second;
			if (action == GLFW_PRESS)
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
			cerr << "Glfw event error : unknown button " << button << endl;
		}
		processEvent(evt);
	}

	void EventGlfw::MotionScroll(GLFWwindow* window, double x, double y)
	{
		if (y>0)
		{
			MouseFunc(window, 100, GLFW_PRESS, last);
			MouseFunc(window, 100, GLFW_RELEASE, last);
		}
		else if (y<0)
		{
			MouseFunc(window, 101, GLFW_PRESS, last);
			MouseFunc(window, 101, GLFW_RELEASE, last);
		}
	}


	void EventGlfw::MotionFunc(GLFWwindow*, double x, double y)
	{
		if (!init) return;

		static Event evt;

		evt.type = Event::EVT_MOUSE_MOVE;
		evt.mod = (Modifier) last;
		evt.mouse.buttons = curr_buttons;
		evt.mouse.x = x;
		evt.mouse.y = y;
		processEvent(evt);
	}

	Event::Modifier EventGlfw::getModifiers(int get)
	{
		static Modifier mod = Modifier::NONE;

		if (get != last)
		{
			static Event evt;
			int up;
			if (last & get)
			{
				cerr << "MOD DOWN" << endl;
				evt.type = Event::EVT_KEYBD_DOWN;
				up = get;
				if (get & GLFW_MOD_CONTROL)	mod = (Modifier) (mod & Event::Modifier::CTRL);
				if (get & GLFW_MOD_SHIFT)  	mod = (Modifier) (mod & Event::Modifier::SHIFT);
				if (get & GLFW_MOD_ALT)			mod = (Modifier) (mod & Event::Modifier::ALT);
				if (get & GLFW_MOD_SUPER)		mod = (Modifier) (mod & Event::Modifier::WINDOW);
			}
			else
			{
				cerr << "MOD UP" << endl;
				evt.type = Event::EVT_KEYBD_UP;
				up = get ^ last;
				if (up & GLFW_MOD_CONTROL)		mod = (Modifier) (mod & ~Event::Modifier::CTRL);
				if (up & GLFW_MOD_SHIFT)		mod = (Modifier) (mod & ~Event::Modifier::SHIFT);
				if (up & GLFW_MOD_ALT)			mod = (Modifier) (mod & ~Event::Modifier::ALT);
				if (up & GLFW_MOD_SUPER)		mod = (Modifier) (mod & ~Event::Modifier::WINDOW);

			}
			if (up & GLFW_MOD_CONTROL)			evt.key = KEY_CTRL;
			else if (up & GLFW_MOD_SHIFT)		evt.key = KEY_SHIFT;
			else if (up & GLFW_MOD_ALT)		evt.key = KEY_ALT;
			else if (up & GLFW_MOD_SUPER)		evt.key = KEY_WINDOW;

			//processEvent(evt);
			last = get;
		}

		return mod;
	}
}
