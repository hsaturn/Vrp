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
#include "Color.h"

using namespace std;

namespace hwidgets
{

	class Coord
	{
	  public:

		Coord() : x(0), y(0), percent(false) { };

		Coord(float _x, float _y, bool absolute)
		:
		x(_x), y(_y), percent(!absolute) { }
		/**
		 * return either 0 or a valid coord depending on data
		 * data = one of these:
		 *      x% y%	relative coords in screen width/height
		 *		x  y    absolute coords in screen width/height
		 */
		static Coord* factory(string &data);

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

		void add(const Coord* c);

	  private:
		int absolute(float coord, int size) const;
		float getPercent(float coord, int size) const;

		Coord(const Coord&);
		Coord& operator=(const Coord&) ;

		float x;
		float y;
		bool percent;
	} ;

	class Rectangle
	{
	  public:

		static Rectangle* factory(string &data);

		bool inside(int x, int y) const
		{
			return x >= x1() && x <= x2() && y >= y1() && y <= y2();
		}

		bool render() const;

		int width() const
		{
			return c2->getAbsoluteX();
		}

		int height() const
		{
			return c2->getAbsoluteY();
		}

		int x1() const
		{
			return c1->getAbsoluteX();
		}

		int x2() const
		{
			return c1->getAbsoluteX() + c2->getAbsoluteX();
		}

		int y1() const
		{
			return c1->getAbsoluteY();
		}

		int y2() const
		{
			return c1->getAbsoluteY() + c2->getAbsoluteY();
		}
		
		void setX(int x) { c1->setAbsoluteX(x); }
		void setY(int y) { c1->setAbsoluteY(y); }
		/*
		void setX(int x) { x1 = x; };
		void setY(int y) { y1 = y; };
		void setWidth(int w) { x2 = x1 + w; }
		void setHeight(int h) { y2 = y1 + h;
		*/

	  private:

		Rectangle() { };

		Rectangle(const Rectangle&);
		Rectangle& operator=(const Rectangle&) ;

		Coord*	c1;
		Coord*	c2;
		const Color*	color;
	} ;

	class Widget
	{
	  public:
		virtual ~Widget();

		const string& getName() const
		{
			return name;
		}
		
		const string& getData() const { return data; }

		/* Return the widget where the mouse is, and inform
		 * the widget that the mouse is moving over it  */
		static Widget* mouseMotion(int x, int y);
		static Widget* mouseButton(int button, int state, int x, int y);

		/* infos = type name rectangle [type dependant data] */
		static Widget* factory(string& infos);

		static long renderAll();
		static bool onCommand(const string&);

		/**
		 * This has to be called whenever the main windows size is changed
		 * @param w
		 * @param h
		 */
		static void handleResize(int w, int h);

		static int screenWidth()
		{
			return screen_width;
		}

		static int screenHeight()
		{
			return screen_height;
		}
		static void pushHelp(string help, string msg = "");
		static void pushMessage(string);
		static void pushEvent(string evt, string msg);
		static string popMessage();

		int width()
		{
			return mrect->width();
		}

		int height()
		{
			return mrect->height();
		}

		const Rectangle* rect() const
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

		static void handleKeypress(unsigned char key, int x, int y);

		static void setCmdQueue(list<string>* p)
		{
			cmdQueue = p;
		}
		static void help(const string& what);

	  protected:

		Widget() : mrect(0) { };
		long render(long ticks);

		/*
		 * mouseOver event, x and y are % relative to widget position
		 */
		virtual void mouseMove(int x, int y, Widget* parent) { };

		virtual void mouseClick(int button, int state, int x, int y) { };

		virtual void keyPress(unsigned char key, int x, int y) { };

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
		Rectangle* mrect;

	  private:
		static list<Widget*>		widgets;
		static Widget*				capture_mouse_widget;
		static Widget*				capture_keybd_widget;
		static Widget*				keyboad_capture;
		static list<string>			pending_messages;
		static map<string, string>	vars;
		static int screen_width;
		static int screen_height;

		string name;
		string data;	// Additionnal widget data

		static map<string, bool>	eventsAllowed;
	} ;

}
#endif /* WIDGET_H */

