/* 
 * File:   WidgetEvent.hpp
 * Author: hsaturn
 *
 * Created on 25 février 2017, 01:20
 */

#ifndef WIDGETEVENT_HPP
#    define WIDGETEVENT_HPP

#    include <stdint.h>
#    include <ostream>

namespace hwidgets
{

	class Event
	{
	  public:

		Event() { };

		virtual ~Event() { };

		class Mouse
		{
		  public:

			typedef enum
			{
				DOWN, UP, CLICK
			} mstate;

			typedef enum
			{
				NONE = 0, LEFT = 1, MIDDLE = 2, RIGHT = 4, WHEEL = 8, OTHER = 16
			} Button;
			Button button;
			int state; // 1 : down, -1 down (for wheel opposite direction)
			int x;
			int y;

			bool operator ==(const Mouse &event) const
			{
				return event.button == button &&
					event.state == state &&
					event.x == x &&
					event.y == y;
			}

			friend std::ostream& operator <<(std::ostream &, const Mouse &);
		};

		class Keybd
		{
		  public:

			typedef enum
			{
				DOWN, UP, PRESS
			} State;

			typedef enum
			{
				NONE = 0,
				L_ALT = 1,
				R_ALT = 2,
				ALT = 3,
				L_SHIFT = 4,
				R_SHIFT = 8,
				SHIFT = 12,
				L_CTRL = 16,
				R_CTRL = 32,
				CTRL = 48,
				MENU = 64,
				WINDOW = 128
			} Modifiers;

			State state;
			uint16_t mod;
			unsigned char key; // ascii
			int mouse_x; // Mouse when event occured (or -1 if unable))
			int mouse_y;

			bool operator ==(const Keybd &event) const
			{
				return event.mod == mod &&
					event.key == key;
			}

			friend std::ostream& operator <<(std::ostream &, const Keybd &);
		};

		Mouse mouse;
		Keybd keybd;
		bool changed;

		/**
		 * 
		 * @return true if event has changed
		 */
		bool update();
		friend std::ostream& operator <<(std::ostream&, const Event&);

	  protected:

		virtual void _update() { };
	};
}

#endif /* WIDGETEVENT_HPP */

