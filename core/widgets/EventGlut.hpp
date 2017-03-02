/*
 * 
 * File:   EventGlut.hpp
 * Author: hsaturn
 *
 * Created on 25 février 2017, 01:26
 */

#ifndef EVENTGLUT_HPP
#    define EVENTGLUT_HPP
#    include "Event.hpp"
#include <queue>

using namespace std;

namespace hwidgets
{

	class EventGlut : public Event
	{
	  public:
		virtual ~EventGlut();

		virtual void update() override;

		static Event* getInstance();
		
	  private:
		EventGlut();
		
		// Glut events handlers
		static void KeyboardFunc(unsigned char, int, int);
		static void KeyboardUpFunc(unsigned char, int, int);
		static void MouseFunc(int, int, int, int);
		static void MotionFunc(int, int);
		static void PassiveMotionFunc(int, int);
		static void SpecialFunc(int key, int x, int y);

		static Modifier getModifiers();

		static bool init;
		static map<uint16_t, uint16_t>	mapButtons;
		static Mouse::Buttons curr_buttons;
	};
}


#endif /* EVENTGLUT_HPP */

