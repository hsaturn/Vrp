#include "Event.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

namespace hwidgets
{
	map<uint16_t, uint16_t> Event::keymap;

	ostream& operator <<(ostream& out, const Event &e)
	{
		out << "Mouse: " << e.mouse << ", Keyboard: " << e.keybd << endl;
		return out;
	}

	void outChar(ostream &out, bool cond, char c)
	{
		if (cond)
			out << c;
		else
			out << ' ';
	}

	ostream& operator << (ostream& out, const Event::Mouse &mouse)
	{
		out << setw(4) << mouse.x << ',' << setw(4) << mouse.y << ' ';
		outChar(out, mouse.button & Event::Mouse::LEFT, 'L');
		outChar(out, mouse.button & Event::Mouse::MIDDLE, 'M');
		outChar(out, mouse.button & Event::Mouse::RIGHT, 'R');
		outChar(out, mouse.button & Event::Mouse::OTHER, 'O');
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

	bool Event::update()
	{
		this->_update();
		bool ret = changed;
		changed = false;
		return ret;
	}

	void Event::readKeymap(string keymapfile)
	{
		int rowcount = 0;
		keymapfile = "data/keymaps/"+keymapfile;
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