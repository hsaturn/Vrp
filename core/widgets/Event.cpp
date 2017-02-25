#include "Event.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace hwidgets
{

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
		bool ret=changed;
		changed = false;
		return ret;
	}

}