#include "Event.hpp"
#include "EventHandler.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

namespace hwidgets
{
	Event* Event::instance = 0;
	map<uint16_t, uint16_t> Event::keymap;
	
	Event::Event()
	{
		if (instance==0)
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
		outChar(out, e.event.type.key_down, 'K');
		outChar(out, e.event.type.key_press, '*');
		outChar(out, e.event.type.key_up, 'k');
		out << ' ';
		outChar(out, e.event.type.mouse_button, 'B');
		outChar(out, e.event.type.mouse_move, '#');
		out << " Mouse: " << e.mouse << ", Keyboard: " << e.keybd << endl;
		return out;
	}


	ostream& operator << (ostream& out, const Event::Mouse &mouse)
	{
		out << setw(4) << mouse.x << ',' << setw(4) << mouse.y << ' ';
		int btn='0';
		for(uint16_t i=1; i<4096; i<<=1, btn++)
			if (mouse.buttons.all & i)
				cout << (char)btn;
			else
				cout << '-';
		cout << ' ';
			
		return out;
	}

	ostream& operator << (ostream& out, const Event::Keybd &kb)
	{
		out << kb.mouse_x << ',' << kb.mouse_y << ' ';
		out << '\'' << kb.key << "' ";
		outChar(out, kb.mod & Event::Keybd::L_ALT, 'A');
		out << '-';
		outChar(out, kb.mod & Event::Keybd::R_ALT, 'A');
		out << ' ';
		outChar(out, kb.mod & Event::Keybd::L_SHIFT, 'S');
		out << '-';
		outChar(out, kb.mod & Event::Keybd::R_SHIFT, 'S');
		out << ' ';
		outChar(out, kb.mod & Event::Keybd::L_CTRL, 'C');
		out << '-';
		outChar(out, kb.mod & Event::Keybd::R_CTRL, 'C');
		out << ' ';
		return out;
	}

	Event* Event::poll()
	{
		Event* ret = 0;
		if (instance)
		{
			instance->_poll();
			EventHandler::dispatch(instance);
			if (instance->event.all)
				ret = instance;
		}
		return ret;
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