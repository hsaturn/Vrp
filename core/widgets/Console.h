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
#include "Draggable.hpp"

namespace hwidgets
{

class Console : public Widget
{
  public:
	virtual ~Console(){};

	static Console* factory(string& data);

  protected:
	virtual long _render(long) override;
	virtual void keyPress(unsigned char key, int x, int y) override;
	virtual bool script(const string& cmd) override;
	virtual void _help(const string& what) override;
	virtual void mouseClick(Event&) override;
	static void listener(const string& send);

  private:
	Console(){};
	Console(const Console&);
	Console& operator=(const Console&);

	string cmd;
	bool redisplay;
	const Color* cmdcolor;
	const Color* anscolor;
	FontRenderer* font;
	static list<string>	display;
};

}

#endif	/* WIDGETCONSOLE_H */

