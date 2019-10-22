/* 
 * File:   Widget.cpp
 * Author: hsaturn
 * 
 * Created on 23 juin 2015, 23:09
 */

#include "Widget.h"

#include <chrono>
#include <iostream>
#include <sstream>

#include <StringUtil.hpp>
#include "Button.h"
#include "Console.h"
#include "EventHandler.hpp"
#include "ansi_colors.hpp"
#include "WidRect.hpp"

namespace hwidgets
{
	WIDGET_REGISTER("button", Button);

	WidRect* screen = 0;
	map<string, bool> Widget::eventsAllowed;
	map<string, string> Widget::vars;
	list<Widget*> Widget::widgets;
	Widget* Widget::capture_mouse_widget = 0;
	Widget* Widget::capture_keybd_widget = 0;
	Widget* Widget::hover_widget = 0;
	list<string> Widget::pending_messages;
	list<string>* Widget::cmdQueue = 0;
	map<Shortcut, Widget*> Widget::shortcuts;
	map<const string, Widget::widgetFuns>* Widget::mapRegisteredWidgets = 0;

	WidRect* getScreen()
	{
		if (screen == 0)
		{
			string data = "0,0 0,0";
			screen = WidRect::factory(data, 0);
			if (screen == 0)
				cerr << Ansi::red() << "getScreen failed" << Ansi::reset() << endl;
		}
		return screen;
	}

	Widget::~Widget()
	{
		if (mrect) delete mrect;
		releaseCaptures();

		// FIXME DELETE FROM list<Widget*>
		// FIXME DELETE FROM map<shortcut, Widget*>
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
		if (bOk)
			pushMessage("#EVENT " + event + ' ' + msg);
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

	Widget* Widget::mouseMotion(Event &event)
	{
		Event::Mouse &mouse = event.mouse;
		hover_widget = findWidget(mouse.x, mouse.y);
		stringstream msg;
		if (hover_widget)
		{
			msg << hover_widget->name;

			hover_widget->mouseMove(event);

			msg << ' ' << (mouse.x - hover_widget->rect()->x1()) << ' ' << (mouse.y - hover_widget->rect()->y1());
		}
		else
			msg << "root";
		msg << ' ' << mouse.x << ' ' << mouse.y;
		pushEvent("mousemove", msg.str());
		return 0;
	}

	void Widget::registerShortcut(Shortcut &a, Widget* w)
	{
		auto it = shortcuts.find(a);
		if (it != shortcuts.end())
		{
			// FIXME dump shortcut
			cerr << "Shortcut already registered." << endl;
		}
		else
			cerr << "SHORTCUT " << a.key << " for " << w->getName() << endl;
		shortcuts[a] = w;
	}

	void Widget::handleKeypress(Event& key)
	{
		Widget* w;
		Shortcut k;
		k.mod = key.mod;
		k.key = key.key;
		auto it = shortcuts.find(k);
		if (it != shortcuts.end())
		{
			cout << "ShortCut" << endl;
			w = it->second;
			Event toto = key;
			toto.mouse.buttons.left = true;
			toto.mouse.button = Event::Mouse::BTN_LEFT;
			toto.type = Event::EVT_MOUSE_DOWN;
			cerr << "Widget::Shortcut " << w->getName() << endl;
			w->mouseClick(toto);
		}
		else
		{
			if (capture_keybd_widget)
				w = capture_keybd_widget;
			else
				w = findWidget(key.mouse.x, key.mouse.y);

			if (w)
				w->keyPress(key);
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
		WidRect* s = getScreen();
		s->setX2(w);
		s->setY2(h);
		pushEvent("resize", StringUtil::to_string(w) + ' ' + StringUtil::to_string(h));
	}

	Widget* Widget::factory(string& infos, Widget* parent)
	{
		Widget* w{nullptr};

		string wid_class = StringUtil::getWord(infos);

		if (wid_class == "help" || wid_class == "")
		{
			pushMessage("widget help:");
			pushMessage("  widget {type} name [font=...] {rectangle} {type args}");
			pushMessage("  widget {type} help        help on widget {type}");
			pushMessage("  widget var name=value");
		}
		else if (wid_class == "var")
		{
			setVar(infos);
			infos = "";
		}
		else if (mapRegisteredWidgets && (mapRegisteredWidgets->find(wid_class) != mapRegisteredWidgets->end()))
		{
			cout << "BUILDING " << wid_class << " WITH FACTORY" << endl;
			string name = StringUtil::getIdentifier(infos);
			auto it = mapRegisteredWidgets->find(wid_class);

			if (name == "help")
				it->second.factory(name);
			else
			{
            const FontRenderer* pFont{nullptr};
            if (infos.substr(0,5)=="font=")
            {
               pFont = FontRendererFactory::build(infos);
            }
            if (pFont == nullptr)
            {
               string default_font = "ft helvetica 10"; // TODO WHAT ?
               pFont = FontRendererFactory::build(default_font);
            }
				WidRect* rect = WidRect::factory(infos, parent ? parent->mrect : 0);

				if (rect)
				{
					w = it->second.factory(infos);
					if (w)
					{
                  w->mpFont = pFont;
						w->mrect = rect;
						w->name = name;
					}
					else
						delete rect;
				}
			}
		}
		else
		{
			cerr << "ERROR WIDGET FACTORY " << wid_class << endl;
		}
		if (w)
		{
			widgets.push_back(w);
			w->data = infos;
		}
		else
		{
			cerr << "ERROR WIDGET FACTORY " << wid_class << " w is null" << endl;
		}
		return w;
	}

	long Widget::render(long ticks)
	{
		if (mrect) mrect->render(this==hover_widget);
		return this->_render(ticks);
	}

	long Widget::renderAll()
	{
		GLfloat normal[4] = {0.5f, 0.5f, 0.5f, 1.0f};
		glDisable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, normal);
      
		static auto t_start = std::chrono::high_resolution_clock::now();
		auto t_now = std::chrono::high_resolution_clock::now();
		long redisplay = 0;

		long ticks = std::chrono::duration<double, std::milli>(t_now - t_start).count();

		for (const auto& widget : widgets)
		{
			long asked = widget->render(ticks);
			if (asked < 0) asked = 0;
			if (redisplay == 0 || (asked < redisplay))
				redisplay = asked;
		}

		return redisplay;
	}

	void Widget::help(const string &what)
	{
		for (const auto& widget : widgets)
			widget->_help(what);
	}

	bool Widget::onCommand(const string& cmd)
	{
		bool ret = false;
		for (const auto& widget : widgets)
		{
			if (widget->name == cmd)
			{
				if (cmd.find(widget->name) != string::npos)
					cerr << "Warning, widget name: " << widget->name << ", used as macro, but cmd (" << cmd << ") contains the name (possible loop)" << endl;
				pushEvent("mousedown", cmd + " -1 " + widget->data);
				ret = true;
			}
			ret |= widget->script(cmd);
		}
		return ret;
	}

	float Coord::getParentWidth() const
	{
		if (parent)
			return parent->width();
		else
			return getScreen()->width();
	}

	float Coord::getParentHeight() const
	{
		if (parent)
			return parent->height();
		else
			return getScreen()->height();
	}

	float Coord::getPercentX() const
	{
		return getPercent(x, percent ? getParentWidth() : 0);
	}

	float Coord::getPercentY() const
	{
		return getPercent(y, getParentHeight());
	}

	float Coord::getPercent(float coord, int size) const
	{
		if (size)
			return coord * 100.0 / (float)size;
		else
			return 0;
	}

	int Coord::getAbsoluteX() const
	{
		return (parent ? parent->x1() : 0) + absolute(x, percent ? getParentWidth() : 0);
	}

	int Coord::getAbsoluteY() const
	{
		return (parent ? parent->y1() : 0) + absolute(y, percent ? getParentHeight() : 0);
	}

	void Coord::setAbsoluteX(int newx)
	{
		if (percent == false)
			x = newx;
		else
			x = absoluteToPercent(newx, percent ? getParentWidth() : 0);
		if (parent)
			x -= parent->x1();
	}

	void Coord::setAbsoluteY(int newy)
	{
		if (percent == false)
			y = newy;
		else
			y = absoluteToPercent(newy, percent ? getParentHeight() : 0);
		if (parent)
			y -= parent->y1();
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
		return coord / ((float)size)*100.0;
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

	Coord* Coord::factory(string& data, WidRect* rect_parent)
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

		Coord* c = new Coord(rect_parent);

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

	void Widget::init()
	{
		static bool done = false;
		if (done)
			cerr << "Widget::init already made" << endl;
		else
		{
			EventHandler::connect(Widget::mouseHandler, Event::Type::EVT_MOUSE_ALL);
			EventHandler::connect(Widget::handleKeypress, Event::Type::EVT_KEYBD_DOWN);
		}
		done = true;
	}

	void Widget::mouseHandler(Event &evt)
	{
		Widget* wid = findWidget(evt.mouse.x, evt.mouse.y);

		if (wid)
			wid->mouseClick(evt);
	}

	void Widget::registerWidgetClass(const string& name, helpFun help, factoryFun factory)
	{
		cerr << "REGISTERING WIDGET CLASS " << name << endl;
		if (mapRegisteredWidgets == 0)
			mapRegisteredWidgets = new map<const string, widgetFuns>;
		if (mapRegisteredWidgets->find(name) != mapRegisteredWidgets->end())
			cerr << "WIDGET ERROR, CLASS " << name << " REGISTERED TWICE (OR MORE)" << endl;
		(*mapRegisteredWidgets)[name] = {help, factory};
	}

	int Widget::screenWidth(WidRect* parent)
	{
		if (parent == 0)
			parent = getScreen();
		return parent->width();
	}

	int Widget::screenHeight(WidRect* parent)
	{
		if (parent == 0)
			parent = getScreen();
		return parent->height();
	}

	int Widget::width()
	{
		return mrect->width();
	}

	int Widget::height()
	{
		return mrect->height();
	}

}
