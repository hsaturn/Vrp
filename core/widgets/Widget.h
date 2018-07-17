/* 
 * File:   Widget.h
 * Author: hsaturn
 *
 * Created on 23 juin 2015, 23:09
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <list>
#include <map>
#include <string>
#include <functional>

#include "Color.h"
#include "Event.hpp"
#include "Widgets.hpp"

using namespace std;

namespace hwidgets
{
	class WidRect;
	
	WidRect* getScreen();
	
	class Coord
	{
	  public:

		Coord(WidRect* rect_parent) : x(0), y(0), percent(false), parent(rect_parent) { };

		Coord(float _x, float _y, bool absolute)
		:
		x(_x), y(_y), percent(!absolute) { }
		/**
		 * return either 0 or a valid coord depending on data
		 * data = one of these:
		 *      x% y%	relative coords in screen width/height
		 *		x  y    absolute coords in screen width/height
		 *		rect	parent if any
		 */
		static Coord* factory(string &data, WidRect* rect_parent);

		static float absoluteToPercent(float coord, int size);

		// return true if absolute x >= this.x
		bool xge(int x) const;

		// return true if absolute y >= this.y
		bool yge(int y) const;
		bool xle(int x) const;
		bool yle(int y) const;
		bool render() const;

		int getAbsoluteX() const;
		int getAbsoluteY() const;

		void setAbsoluteX(int x);
		void setAbsoluteY(int y);

		float getPercentX() const;
		float getPercentY() const;

		bool isAbsolute() const
		{
			return !percent;
		};
		
		float getParentWidth() const;
		float getParentHeight() const;

		void add(const Coord* c);
		
	  private:
		int absolute(float coord, int size) const;
		float getPercent(float coord, int size) const;

		Coord(const Coord&);
		Coord& operator=(const Coord&) ;

		float x;
		float y;
		bool percent;
		WidRect* parent;
	};

	class Widget
	{
	  public:
		typedef function<void(const string& what)> helpFun;
		typedef function<Widget* (string &data)> factoryFun;
		typedef struct 
		{
			helpFun help;
			factoryFun factory;
		} widgetFuns;
		
		virtual ~Widget();

		const string& getName() const
		{
			return name;
		}
		
		const string& getData() const { return data; }
		
		static void registerWidgetClass(const string& wclass, helpFun, factoryFun);

		/* Return the widget where the mouse is, and inform
		 * the widget that the mouse is moving over it  */
		static Widget* mouseMotion(Event&);
		static Widget* mouseButton(int button, int state, int x, int y);
		
		static void registerShortcut(Shortcut &a, Widget* w);

		/* infos = type name rectangle [type dependant data] */
		static Widget* factory(string& infos, Widget* parent=0);

		static long renderAll();
		static bool onCommand(const string&);

		/**
		 * This has to be called whenever the main windows size is changed
		 * @param w
		 * @param h
		 */
		static void handleResize(int w, int h);

		static int screenWidth(WidRect* parent = 0);

		static int screenHeight(WidRect* parent = 0);
	
		static void pushHelp(string help, string msg = "");
		static void pushMessage(string);
		static void pushEvent(string evt, string msg);
		static string popMessage();

		int width();
		int height();

		const WidRect* rect() const
		{
			return mrect;
		}

		static Widget* findWidget(int x, int y);
		static Widget * findWidget(string name);

		static void filtersOp(string events, bool filtered);
		static void replaceVars(string &text);
		static void setVar(string);
		static string getVar(string);
		static void clear();


		static void setCmdQueue(list<string>* p)
		{
			cmdQueue = p;
		}
		static void help(const string& what);
		static void init();

		
	  protected:

		Widget() : mrect(0) { };
		long render(long ticks);
		
		static void handleKeypress(Event &key);
		static void mouseHandler(Event &);

		/*
		 * mouseOver event, x and y are % relative to widget position
		 */
		virtual void mouseMove(Event &) { };

		virtual void mouseClick(Event &) { };

		virtual void keyPress(Event& key){};

		virtual bool script(const string& s)
		{
			return false;
		};

		virtual void _help(const string& what) { };

		/**
		 * @param ms tick count
		 * @return ms 0 or delay (ms) request for a new rendering
		 */
		virtual long _render(long ms) = 0;

		//
		void pushCmdFront(string s);

		void setName(const string n)
		{
			name = n;
		}

		static list<string>*		cmdQueue;
		
		bool mouseCapture();
		void mouseRelease();
		void releaseCaptures();
		
	  protected:
		WidRect*	mrect;

	  private:
		static list<Widget*>		widgets;
		static Widget*				capture_mouse_widget;
		static Widget*				capture_keybd_widget;
		static Widget*				hover_widget;
		static Widget*				keyboad_capture;
		static list<string>			pending_messages;
		static map<string, string>	vars;

		string name;
		string data;	// Additionnal widget data

		static map<string, bool>		eventsAllowed;
		static map<Shortcut, Widget*>	shortcuts;
		
		static map<const string, widgetFuns>	*mapRegisteredWidgets;
		
	};
	
#define WIDGET_REGISTER(name, wclass) \
	class WidgetRegistration##wclass{\
		public:\
		WidgetRegistration##wclass() {\
			Widget::registerWidgetClass(name, wclass::help, wclass::factory);\
		}\
	};\
	WidgetRegistration##wclass _widgetRegistration##wclass;
	
}
#endif /* WIDGET_H */

