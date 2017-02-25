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

namespace hwidgets
{

	class EventGlut : public Event
	{
	  public:
		EventGlut();
		virtual ~EventGlut();
		
		virtual void _update() override
		{
		}
		
	  private:
		// Glut events handlers
		static void KeyboardFunc(unsigned char, int, int);
		static void KeyboardUpFunc(unsigned char, int, int);
		static void MouseFunc(int, int, int, int);
		static void MotionFunc(int, int);
		static void PassiveMotionFunc(int, int);

		void updateModifiers();

		static EventGlut* instance;
		static bool init;
	};
}


#endif /* EVENTGLUT_HPP */

