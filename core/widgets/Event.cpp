#include "Event.hpp"
#include "EventHandler.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

namespace hwidgets {
	Event* Event::instance = 0;
	map<uint16_t, uint16_t> Event::keymap;
	queue<Event> Event::events;

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

	void Event::poll(Event &evt)
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
			}
		}
	}

	void Event::readKeymap(string keymapfile)
	{
		int rowcount = 0;
		keymapfile = "data/keymaps/" + keymapfile;
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
				if (keymap.find(key) != keymap.end())
				{
					cerr << "Keymap file " << keymapfile << ", warning duplicate entry at row :" << rowcount << endl;
				}
				keymap[key] = mapped;
			}

		}
	}

}
