/* 
 * File:   Widget.cpp
 * Author: hsaturn
 * 
 * Created on 23 juin 2015, 23:09
 */

#include "Widget.h"
#include <StringUtil.hpp>
#include "Button.h"
#include "Console.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <chrono>


namespace hwidgets
{
	map<string, bool>	Widget::eventsAllowed;
	map<string, string> Widget::vars;
	list<Widget*> Widget::widgets;
	Widget*	Widget::capture_mouse_widget = 0;
	Widget* Widget::capture_keybd_widget = 0;
	list<string> Widget::pending_messages;
	list<string>* Widget::cmdQueue = 0;

	int Widget::screen_width = -1;
	int Widget::screen_height = -1;

	Widget::~Widget()
	{
		if (mrect) delete mrect;
		releaseCaptures();
	}

	void Widget::clear()
	{
		eventsAllowed.clear();
		pending_messages.clear();
		vars.clear();
		widgets.clear();
	}

	void Widget::pushEvent(string event, string msg)
	{
		bool bOk = true;
		if (eventsAllowed.size())
		{
			auto it = eventsAllowed.find(event);
			if (it != eventsAllowed.end())
				bOk = it->second;
		}
		if (bOk) pushMessage("#EVENT " + event + ' ' + msg);
	}

	void Widget::pushHelp(string help, string msg)
	{
		pending_messages.push_back("   " + help + "  " + msg);
	}

	void Widget::pushMessage(string msg)
	{
		pending_messages.push_back(msg);
	}

	string Widget::popMessage()
	{
		string msg;
		if (pending_messages.size())
		{
			msg = pending_messages.front();
			pending_messages.pop_front();
		}
		return msg;
	}

	Widget* Widget::mouseButton(int button, int state, int x, int y)
	{
		Widget* wid = findWidget(x, y);

		if (wid)
		{
			wid->mouseClick(button, state, x, y);
		}
		return 0;
	}

	Widget* Widget::mouseMotion(int x, int y)
	{
		Widget* wid = findWidget(x, y);
		string msg;
		if (wid)
		{
			msg = wid->name;

			wid->mouseMove(x, y, 0);

			msg += " " + StringUtil::to_string(x - wid->rect()->x1()) + ' ' + StringUtil::to_string(y - wid->rect()->y1());
		}
		else
			msg = "root";
		msg += " " + StringUtil::to_string(x) + ' ' + StringUtil::to_string(y);
		pushEvent("mousemove", msg);
		return 0;
	}

	void Widget::handleKeypress(unsigned char key, int x, int y)
	{
		Widget* w;
		if (capture_keybd_widget)
			w = capture_keybd_widget;
		else
			w = findWidget(x, y);
		if (w)
		{
			w->keyPress(key, x, y);
		}
	}

	Widget* Widget::findWidget(int x, int y)
	{
		if (capture_mouse_widget)
			return capture_mouse_widget;

		for (auto it = widgets.rbegin(); it != widgets.rend(); it++)
			if ((*it)->mrect->inside(x, y)) return *it;

		return 0;
	}

	void Widget::handleResize(int w, int h)
	{
		screen_width = w;
		screen_height = h;
		pushEvent("resize", StringUtil::to_string(w) + ' ' + StringUtil::to_string(h));
	}

	Widget* Widget::factory(string& infos)
	{
		Widget* w = 0;

		string type = StringUtil::getWord(infos);
		if (type == "help" || type == "")
		{
			pushMessage("widget help:");
			pushMessage("  widget {type} name {args}      type=button");
			pushMessage("  widget {type} help        help on widget {type}");
			pushMessage("  widget var name=value");
		}
		else if (type == "var")
		{
			setVar(infos);
			infos = "";
		}
		else if (type == "button")
		{
			string name = StringUtil::getIdentifier(infos);

			if (name == "help")
			{
				Button::factory(name);
			}
			else
			{
				Rectangle* rect = Rectangle::factory(infos);

				if (rect)
				{
					w = Button::factory(infos);
					if (w)
					{
						w->mrect = rect;
						w->name = name;
					}
					else
						delete rect;
				}
			}
		}
		else if (type == "console")
		{
			Rectangle* rect = Rectangle::factory(infos);

			if (rect)
			{
				Console* cons = Console::factory(infos);
				if (cons)
				{
					cons->mrect = rect;
					w = cons;
				}
				else
				{
					cerr << "console cons is null" << endl;
					delete cons;
				}
			}
			else
			{
				cerr << "console rect is null" << endl;
			}
		}
		else
		{
			cerr << "ERROR WIDGET FACTORY " << type << endl;
		}
		if (w)
		{
			widgets.push_back(w);
			w->data = infos;
		}
		else
		{
			cerr << "ERROR WIDGET FACTORY " << type << " w is null" << endl;
		}
		return w;
	}

	long Widget::render(long ticks)
	{
		if (mrect) mrect->render();
		return this->_render(ticks);
	}

	long Widget::renderAll()
	{
		GLfloat normal[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		glDisable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, normal);
		static auto t_start = std::chrono::high_resolution_clock::now();
		auto t_now = std::chrono::high_resolution_clock::now();
		long redisplay = 0;

		long ticks = std::chrono::duration<double, std::milli>(t_now - t_start).count();

		for (auto it = widgets.begin(); it != widgets.end(); it++)
		{
			long asked = (*it)->render(ticks);
			if (asked < 0) asked = 0;
			if (redisplay == 0 || (asked < redisplay))
				redisplay = asked;
		}

		return redisplay;
	}

	void Widget::help(const string &what)
	{
		for (auto it = widgets.begin(); it != widgets.end(); it++)
			(*it)->_help(what);
	}

	bool Widget::onCommand(const string& cmd)
	{
		bool ret = false;
		for (auto it = widgets.begin(); it != widgets.end(); it++)
		{
			if ((*it)->name == cmd)
			{
				if (cmd.find((*it)->name) != string::npos)
					cerr << "Warning, widget name used as macro, but cmd contains the name (possible loop)" << endl;
				pushEvent("mousedown", cmd + " -1 " + (*it)->data    );
				ret = true;
			}
			ret |= (*it)->script(cmd);
		}
		return ret;
	}

	float Coord::getPercentX() const
	{
		return getPercent(x, Widget::screenWidth());
	}

	float Coord::getPercentY() const
	{
		return getPercent(y, Widget::screenHeight());
	}

	float Coord::getPercent(float coord, int size) const
	{
		if (size)
			return coord * 100.0 / (float) size;
		else
			return 0;
	}

	int Coord::getAbsoluteX() const
	{
		return absolute(x, Widget::screenWidth());
	}

	int Coord::getAbsoluteY() const
	{
		return absolute(y, Widget::screenHeight());
	}

	void Coord::setAbsoluteX(int newx)
	{
		if (percent == false)
			x = newx;
		else
			x = absoluteToPercent(newx, Widget::screenWidth());
	}

	void Coord::setAbsoluteY(int newy)
	{
		if (percent == false)
			y = newy;
		else
			y = absoluteToPercent(newy, Widget::screenHeight());
	}

	int Coord::absolute(float coord, int size) const
	{
		if (percent)
			return size * coord / 100.0;
		else
			return coord;
	}

	float Coord::absoluteToPercent(float coord, int size)
	{
		return coord / ((float) size)*100.0;
	}

	bool Coord::xge(int x) const
	{
		return x >= getAbsoluteX();
	}

	bool Coord::xle(int x) const
	{
		return x <= getAbsoluteX();
	}

	bool Coord::yge(int y) const
	{
		return y >= getAbsoluteY();
	}

	bool Coord::yle(int y) const
	{
		return y <= getAbsoluteY();
	}

	bool Coord::render() const
	{
		glVertex2i(getAbsoluteX(), getAbsoluteY());
		return false;
	}

	Coord* Coord::factory(string& data)
	{
		static int x = 0;
		static int y = 0;

		bool relative = false;
		bool start_relative = false;

		if (data.substr(0, 4) == "help")
		{
			Widget::pushMessage("Coord help:");
			Widget::pushMessage("  x,y               Absolute coords");
			Widget::pushMessage("  =x,=y             Set origin of relative coords");
			Widget::pushMessage("  +-x,+-y           Relative to origin");
			Widget::pushMessage("  x%,y%             Screen percentage coords");
			return 0;
		}

		string sOrg = data;
		bool mix_percent = false;

		Coord* c = new Coord();

		// X
		relative = (data[0] == '+' || data[0] == '-');
		start_relative = (data[0] == '=');
		if (start_relative) data.erase(0, 1);

		c->x = StringUtil::getFloat(data);
		c->percent = data[0] == '%';
		if (c->percent)
			data.erase(0, 1);
		else if (relative)
			c->x += x;
		if (relative || start_relative) x = c->x;
		if (data[0] != ',') return 0;
		data.erase(0, 1);


		// Y
		relative = (data[0] == '+' || data[0] == '-');
		start_relative = (data[0] == '=');
		if (start_relative) data.erase(0, 1);

		c->y = StringUtil::getFloat(data);
		if (c->percent != (data[0] == '%'))
			mix_percent = true;
		else if (c->percent)
			data.erase(0, 1);

		if (relative && !c->percent) c->y += y;
		if (relative || start_relative) y = c->y;

		if (mix_percent)
		{
			cerr << "Coord::Factory, Mixing % and absolute in coords (data)";
			delete c;
			c = 0;
		}
		if (c == 0) cerr << "Coord::Factory failed(" << data << ")" << endl;
		return c;
	}

	void Coord::add(const Coord* c)
	{
		if (c == 0) return;
		if (percent != c->percent)
		{
			if (percent)
			{
				x += c->getPercentX();
				y += c->getPercentY();
				cout << "cas A " << x << ',' << y << endl;
			}
			else
			{

				x += c->getAbsoluteX();
				y += c->getAbsoluteY();
				cout << "cas B " << x << ',' << y << endl;
			}
		}
		else
		{
			cout << "cas C " << x << ',' << y << endl;
			x += c->x;
			y += c->y;
		}
	}

	Rectangle* Rectangle::factory(string &infos)
	{
		Rectangle* rect;

		if (infos.substr(0, 4) == "help")
		{
			Widget::pushMessage("rectangle help");
			Widget::pushMessage(" top_left_coord top_right_coord");
			Coord::factory(infos); // Help for coord
		}
		else
		{
			rect = new Rectangle;
			rect->c1 = Coord::factory(infos);
			rect->c2 = Coord::factory(infos);
			rect->color = Color::factory(infos);
			if (rect->c1 && rect->c2)
			{
			}
			else
			{
				cerr << "bad rectangle" << endl;
				delete rect;
				rect = 0;
			}
		}
		return rect;
	}

	bool Rectangle::render() const
	{
		glBegin(GL_QUADS);
		if (color)
			color->render();
		else
			Color::dark_gray.render();
		int x(c1->getAbsoluteX());
		int y(c1->getAbsoluteY());
		glVertex2i(x, y);
		glVertex2i(x + c2->getAbsoluteX(), y);
		glVertex2i(x + c2->getAbsoluteX(), y + c2->getAbsoluteY());
		glVertex2i(x, y + c2->getAbsoluteY());
		glEnd();
		return false;
	}

	void Widget::filtersOp(string events, bool filtered)
	{
		while (events.length())
		{
			string event(StringUtil::getWord(events));
			if (event.length())
			{
				eventsAllowed[event] = !filtered;
			}
		}
	}

	void Widget::replaceVars(string& text)
	{
		for (auto it = vars.begin(); it != vars.end(); it++)
		{
			string name = string("$") + it->first;

			string::size_type pos = string::npos;
			while (text.find(name) != pos)
			{
				pos = text.find(name);
				if (pos != string::npos)
					text.replace(pos, name.length(), it->second);
			}
		}

	}

	string Widget::getVar(string name)
	{
		auto it = vars.find(name);
		if (it != vars.end())
			return it->second;
		return "";
	}

	void Widget::setVar(string def)
	{
		string name(StringUtil::getWord(def, '='));
		if (def[0] == '"' || def[0] == '\"')
			def = StringUtil::getString(def, true);
		vars[name] = def;
	}

	bool Widget::mouseCapture()
	{
		if (capture_mouse_widget != this)
		{
			capture_mouse_widget = this;
			cout << "Capturing mouse" << endl;
			return true;
		}
		return false;
	}

	void Widget::mouseRelease()
	{
		if (capture_mouse_widget)
		{
			capture_mouse_widget = 0;
			cout << "Releasing mouse" << endl;
		}
	}

	void Widget::releaseCaptures()
	{
		if (capture_mouse_widget == this) capture_mouse_widget = 0;
		if (capture_keybd_widget == this) capture_keybd_widget = 0;
	}
}

