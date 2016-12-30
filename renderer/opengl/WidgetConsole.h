/* 
 * File:   WidgetConsole.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 13:34
 */

#ifndef WIDGETCONSOLE_H
#define	WIDGETCONSOLE_H

#include "Widget.h"
#include "Color.h"
#include "FontRenderer.h"

namespace hwidgets
{

class WidgetConsole : public Widget
{
  public:
	virtual ~WidgetConsole(){};

	static WidgetConsole* factory(string& data);

  protected:
	virtual long _render(long);
	virtual void keyPress(unsigned char key, int x, int y);
	virtual bool script(const string& cmd);
	virtual void _help(const string& what);
	virtual void mouseClick(int button, int state, int x, int y);
	static void listener(const string& send);

  private:
	WidgetConsole(){};
	WidgetConsole(const WidgetConsole&);
	WidgetConsole& operator=(const WidgetConsole&);

	string cmd;
	bool redisplay;
	const Color* cmdcolor;
	const Color* anscolor;
	FontRenderer* font;
	static list<string>	display;
};

}

#endif	/* WIDGETCONSOLE_H */

