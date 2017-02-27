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
#    include <fstream>
#    include <sstream>
#    include <iostream>
#include <queue>

using namespace std;

namespace hwidgets
{

	class Event
	{
	  public:

		Event();

		virtual ~Event() { };

		typedef union EventType
		{

			struct
			{
				bool mouse_move : 1; // Mouse has moved
				bool mouse_button : 1;
				bool mouse_button_up : 1; // Mouse button state changed
				bool mouse_button_down : 1;
				bool key_up : 1;
				bool key_down : 1;
				bool key_press : 1;
			} type;
			uint8_t all;

			friend bool operator ==(const EventType& p, const EventType &q)
			{
				return p.all == q.all;
			}

			friend bool operator !=(const EventType& p, const EventType &q)
			{
				return p.all == q.all;
			}

			template <class KEY, class DATA>
			bool readEvt(string file, map<KEY, DATA> &);
		} EventType;

		EventType event;

		class Mouse
		{
		  public:
			static const uint16_t NONE = 0;
			static const uint16_t LEFT = 1;
			static const uint16_t MIDDLE = 2;
			static const uint16_t RIGHT = 4;
			static const uint16_t WHEEL_UP = 8;
			static const uint16_t WHEEL_DOWN = 16;
			static const uint16_t BTN_5 = 32;
			static const uint16_t BTN_6 = 64;
			static const uint16_t BTN_7 = 128;
			static const uint16_t BTN_8 = 256;
			static const uint16_t BTN_9 = 512;
			static const uint16_t BTN_10 = 1024;
			static const uint16_t BTN_11 = 2048;
			static const uint16_t BTN_12 = 4096;

			// Last Modified button
			uint16_t last_button;
			bool button_down;

			/**
			 * Value of all mouse buttons
			 */
			union
			{

				struct
				{
					bool left : 1;
					bool middle : 1;
					bool right : 1;
					bool wheel_up : 1;
					bool wheel_down : 1;
					bool btn_5 : 1;
					bool btn_6 : 1;
					bool btn_7 : 1;
					bool btn_8 : 1;
					bool btn_9 : 1;
					bool btn_10 : 1;
					bool btn_11 : 1;
					bool btn_12 : 1;
				};
				uint16_t all;
			} buttons;

			/**
			 * Number of last event button
			 */
			uint16_t button;

			int x;
			int y;

			bool operator ==(const Mouse &event) const
			{
				return event.buttons.all == buttons.all &&
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

		static void init(Event* inst)
		{
			instance = inst;
		}
		/**
		 * 
		 * @return ptr on event when occurs
		 */
		static Event* poll();
		friend std::ostream& operator <<(std::ostream&, const Event&);

	  protected:

		virtual void _poll() { };

		static void readKeymap(string keymapfile);
		static map<uint16_t, uint16_t> keymap;
		static queue<Event> events;
		static Event* instance;
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

	template <class KEY, class DATA>
	bool readEvt(string file, map<KEY, DATA> &values)
	{
		file = "data/core/events/" + file + ".evt";
		ifstream evt(file.c_str());
		if (evt.is_open())
		{
			while (evt.good())
			{
				string row;
				getline(evt, row);
				if (row.length() && row[0] != '#')
				{
					KEY key;
					DATA data;
					stringstream rd;
					rd << row;
					rd >> key;
					rd >> data;
					if (values.find(key) != values.end())
						cerr << "Warning: duplicate entry " << key << " in file " << file << "." << endl;
					values[key] = data;
				}
			}
			return true;
		}
		else
			cerr << "ERROR: Unable to open event data file " << file << endl;

		return false;
	}
}

#endif /* WIDGETEVENT_HPP */

