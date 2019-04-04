/* 
 * File:   WidgetEvent.hpp
 * Author: hsaturn
 *
 * Created on 25 février 2017, 01:20
 */

#ifndef WIDGETEVENT_HPP
#define WIDGETEVENT_HPP

#include <stdint.h>
#include <ostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>

using namespace std;

namespace hwidgets
{

	class Event
	{
	  public:

		Event();

		virtual ~Event() { };

		enum Type
		{
			EVT_NONE		= 0,
			EVT_MOUSE_MOVE	= 1,
			EVT_MOUSE_DOWN	= 2,
			EVT_MOUSE_UP	= 4,

			EVT_MOUSE_ALL  = EVT_MOUSE_MOVE | EVT_MOUSE_DOWN | EVT_MOUSE_UP,

			EVT_KEYBD_DOWN   = 8,
			EVT_KEYBD_PRESS  = 16,
			EVT_KEYBD_UP	 = 32,

			EVT_KEY_ALL	= EVT_KEYBD_DOWN | EVT_KEYBD_PRESS | EVT_KEYBD_UP,
		} ;

		Type type;

		template <class KEY, class DATA>
		static bool readEvt(string file, map<KEY, DATA> &);

		class Mouse
		{
		  public:

			enum Button
			{
				BTN_NONE = 0,
				BTN_LEFT = 1,
				BTN_MIDDLE = 2,
				BTN_RIGHT = 4,
				BTN_WHEEL_UP = 8,
				BTN_WHEEL_DOWN = 16,
				BTN_5 = 32,
				BTN_6 = 64,
				BTN_7 = 128,
				BTN_8 = 256,
				BTN_9 = 512,
				BTN_10 = 1024,
				BTN_11 = 2048,
				BTN_12 = 4096,
			} ;

			uint16_t button;

			/**
			 * Value of all mouse buttons
			 */
			typedef union Buttons
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
				} ;
				uint16_t all;
			} Buttons;
			
			Buttons buttons;
			
			int x;
			int y;

			friend std::ostream& operator <<(std::ostream &, const Mouse &) ;
		} ;

		typedef enum Modifier : uint16_t
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
			WINDOW = 128,
		} Modifier;

		Modifier mod;

		Mouse mouse;

		enum Key : uint16_t
		{
			KEY_F1 = 0x101,
			KEY_F2 = 0x102,
			KEY_F3 = 0x103,
			KEY_F4 = 0x104,
			KEY_F5 = 0x105,
			KEY_F6 = 0x106,
			KEY_F7 = 0x107,
			KEY_F8 = 0x108,
			KEY_F9 = 0x109,
			KEY_F10 = 0x10a,
			KEY_F11 = 0x10b,
			KEY_F12 = 0x10c,
			KEY_F13 = 0x10d,

			KEY_UP = 0x110,
			KEY_LEFT = 0x111,
			KEY_DOWN = 0x112,
			KEY_RIGHT = 0x113,

			KEY_PGDOWN = 0x114,
			KEY_PGUP = 0x115,

			KEY_HOME = 0x120,
			KEY_END = 0x121,
			KEY_INSERT = 0x122,
			
			// 0x200 + Modifier
			KEY_CTRL = 0x230,
			KEY_SHIFT = 0x20C,
			KEY_RSHIFT = 0x270,
			KEY_LSHIFT = 0x271,
			KEY_LCTRL = 0x272,
			KEY_RCTRL = 0x273,
			KEY_ALT = 0x203,
			KEY_MENU = 0x240,
			KEY_WINDOW = 0x280,
		};

		uint16_t key; // ascii or const (see below)

		/**
		 * @param inst Event manager
		 * @param polling_mode true if pool mode, else, use EventHandler::connect to
		 * handle events.
		 */
		static void init(Event* inst, bool polling_mode = false)
		{
			instance = inst;
			_polling_mode = polling_mode;
		}
		/**
		 * 
		 * @return ptr on event when occurs
		 */
		static bool poll(Event &e);
		
		friend std::ostream& operator <<(std::ostream&, const Event&) ;

	  protected:
		
		/**
		 * Derived classes call this to process event
		 * This method will either store or dispatch the event.
		 * @param event
		 */
		static void processEvent(Event& event);

		virtual void update() { };

		static void readMap(string keymapfile, map<uint16_t, uint16_t>&);
		static map<uint16_t, uint16_t> keymap;
		static Event* instance;
		
	  private:
		static bool _polling_mode;
		static queue<Event> events;
	} ;

	template <class KEY, class DATA>
	bool Event::readEvt(string file, map<KEY, DATA> &values)
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
	
	class Shortcut
	{
	  public:
		Event::Modifier mod;
		uint16_t key;
		
		/**
		 * 
		 * @param  s = [MOD[+MOD...]]key
		 *         mod = ALT | LALT | RALT | SHIFT | LSHIFT | RSHIFT | CTRL | RCTRL | LCTRL | WIN | MENU
		 *         key = 0xXXX | 'X' | nnn
		 * @return true if s is a valid shortcut string 
		 */
		bool set(string&);
		
		
		bool operator == (const Event& evt)
		{
			return evt.mod == mod && evt.key == key;
		}
		
		bool operator == (const Shortcut &c)
		{
			return c.mod == mod && c.key == key;
		}
	  private:
		void checkMod(string& s, const string key, Event::Modifier newmod);
	};

	bool operator < (const Shortcut& a, const Shortcut& b);
}

#endif /* WIDGETEVENT_HPP */

