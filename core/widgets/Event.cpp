#include "Event.hpp"
#include "EventHandler.hpp"
#include "StringUtil.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

namespace hwidgets
{
	Event* Event::instance = 0;
	map<uint16_t, uint16_t> Event::keymap;
	queue<Event> Event::events;
	bool Event::_polling_mode = false;

	Event::Event()
	{
		if (instance == 0)
		{
			cerr << "WARNING: Event created, but event manager has not been set." << endl;
		}
	}

	void outChar(ostream &out, bool cond, char c)
	{
		if (cond)
			out << c;
		else
			out << ' ';
	}

	ostream& operator <<(ostream& out, const Event &e)
	{
		out << "Evt: ";
		outChar(out, e.type == Event::EVT_KEYBD_DOWN, 'K');
		outChar(out, e.type == Event::EVT_KEYBD_PRESS, '*');
		outChar(out, e.type == Event::EVT_KEYBD_UP, 'k');
		out << ' ';
		outChar(out, e.type == Event::EVT_MOUSE_DOWN, 'B');
		outChar(out, e.type == Event::EVT_MOUSE_MOVE, '#');
		out << " Mouse: " << e.mouse << ", Keyboard: " ;

		out << '\'' << e.key << "' " << (uint16_t) e.key << ' ';
		outChar(out, e.mod & Event::ALT, 'A');
		out << '-';
		outChar(out, e.mod & Event::R_ALT, 'A');
		out << ' ';
		outChar(out, e.mod & Event::L_SHIFT, 'S');
		out << '-';
		outChar(out, e.mod & Event::R_SHIFT, 'S');
		out << ' ';
		outChar(out, e.mod & Event::L_CTRL, 'C');
		out << '-';
		outChar(out, e.mod & Event::R_CTRL, 'C');
		out << ' ';
		return out;
	}

	ostream& operator << (ostream& out, const Event::Mouse &mouse)
	{
		out << setw(4) << mouse.x << "x, " << setw(4) << mouse.y << "y Btns: ";
		int btn = '0';
		for (uint16_t i = 1; i < 4096; i <<= 1, btn++)
			if (mouse.buttons.all & i)
				cout << (char) btn;
			else
				cout << '-';
		cout << ' ';

		return out;
	}

	bool Event::poll(Event &evt)
	{
		if (instance)
		{
			evt.type = EVT_NONE;
			instance->update();
			if (events.size())
			{
				evt = events.front();
				events.pop();
				EventHandler::dispatch(evt);
				return evt.type != EVT_NONE;
			}
		}
		return false;
	}

	void Event::processEvent(Event& event)
	{
		if (_polling_mode)
			events.push(event);
		else
			EventHandler::dispatch(event);
	}

	void Event::readMap(string keymapfile, map<uint16_t, uint16_t>& kmap)
	{
		int rowcount = 0;
		keymapfile = "data/core/events/" + keymapfile;
		ifstream kfile(keymapfile);
		if (!kfile.is_open())
		{
			cerr << "ERROR, keymap file not found : " << keymapfile << "." << endl;
			return;
		}
		while (kfile.good())
		{
			uint16_t key;
			uint16_t mapped;

			string row;

			getline(kfile, row);
			rowcount++;
			if (row.length() && row[0] != '#')
			{
				stringstream strow;
				strow << row;
				strow >> key;
				strow >> mapped;
				if (mapped == 0 && !strow.eof() && strow.peek() == 'x')
				{
					char c;
					strow >> c;
					strow >> hex >> mapped >> dec;
				}
				else
					cout << "key " << key << " mapped:" << mapped << endl;
				if (kmap.find(key) != kmap.end())
				{
					cerr << "Keymap file " << keymapfile << ", warning duplicate entry at row :" << rowcount << endl;
				}
				kmap[key] = mapped;
			}

		}
	}

	bool operator < (const Shortcut& a, const Shortcut& b)
	{
		if (a.mod < b.mod)
			return true;
		if (a.mod > b.mod)
			return false;
		return a.key < b.key;
	}

	void Shortcut::checkMod(string& s, const string key, Event::Modifier newmod)
	{
		if (s.length() >= key.length())
		{
			if (s.substr(0, key.length()) == key)
			{
				cout << "=" << key << endl;
				mod = (Event::Modifier)(mod | newmod);
				s.erase(0, key.length());
				if (s[0] == '+')
					s.erase(0, 1);
			}
		}
	}

	bool Shortcut::set(string & skey)
	{
		mod = (Event::Modifier)0;
		string::size_type t = 0;
		while (skey.length() != t)
		{
			checkMod(skey, "ALT"	, Event::Modifier::ALT);
			checkMod(skey, "LALT"	, Event::Modifier::L_ALT);
			checkMod(skey, "RALT"	, Event::Modifier::R_ALT);
			checkMod(skey, "SHIFT"	, Event::Modifier::SHIFT);
			checkMod(skey, "LSHIFT"	, Event::Modifier::L_SHIFT);
			checkMod(skey, "RSHIFT"	, Event::Modifier::R_SHIFT);
			checkMod(skey, "MENU"	, Event::Modifier::MENU);
			checkMod(skey, "WIN"	, Event::Modifier::WINDOW);

			if (skey.substr(0, 2) == "0x")
				key = StringUtil::getLong(skey);

			if (skey[0] == '\'')	// FIXME dual usage with 0x
				if (skey.length() >= 3 && skey[2] == '\'')
				{
					key = skey[1];
					skey.erase(0, 3);
				}
			if (skey[0] == ' ')
				skey.erase(0, 1);

			t = skey.length();

		}
		// FIXME detect if key not set
		if (skey.length())
		{
			// FIXME bad error reporting
			cerr << "Error syntax in shortcut (" << skey << ")." << endl;
			return false;
		}
		return true;
	}
}