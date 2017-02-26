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
#    include <map>
#    include <string>

using namespace std;

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
			uint16_t key; // ascii or const (see below)
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

		static void readKeymap(string keymapfile);
		static map<uint16_t, uint16_t> keymap;
	};

	const int KEY_F1 = 0x101;
	const int KEY_F2 = 0x102;
	const int KEY_F3 = 0x103;
	const int KEY_F4 = 0x104;
	const int KEY_F5 = 0x105;
	const int KEY_F6 = 0x106;
	const int KEY_F7 = 0x107;
	const int KEY_F8 = 0x108;
	const int KEY_F9 = 0x109;
	const int KEY_F10 = 0x10a;
	const int KEY_F11 = 0x10b;
	const int KEY_F12 = 0x10c;
	const int KEY_F13 = 0x10d;

	const int KEY_UP = 0x110;
	const int KEY_LEFT = 0x111;
	const int KEY_DOWN = 0x112;
	const int KEY_RIGHT = 0x113;

	const int KEY_PGDOWN = 0x114;
	const int KEY_PGUP = 0x115;

	const int KEY_HOME = 0x120;
	const int KEY_END = 0x121;
	const int KEY_INSERT = 0x122;
}

#endif /* WIDGETEVENT_HPP */

