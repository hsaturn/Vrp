/*
 * 
 * File:   EventGlfw.hpp
 * Author: hsaturn
 *
 * Created on 25 février 2017, 01:26
 */
#pragma once

#include "Event.hpp"
#include <GLFW/glfw3.h>
#include <queue>

using namespace std;

namespace hwidgets
{

	class EventGlfw : public Event
	{
	  public:
		virtual ~EventGlfw();

		virtual void update() override;

		static Event* getInstance(GLFWwindow *);
		
	  private:
		EventGlfw();
		
		// Glfw events handlers
		static void KeyboardFunc(GLFWwindow*,int key, int scan, int action, int mods);
		static void MouseFunc(GLFWwindow*, int button, int action, int mods);
		static void MotionFunc(GLFWwindow*, double, double);

		static Modifier getModifiers(int mods);

		static bool init;
		static map<uint16_t, uint16_t>	mapButtons;
		static Mouse::Buttons curr_buttons;
		static int last;
	};
}
