/* 
 * File:   WidgetConsole.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 13:34
 */

#ifndef WIDGETCONSOLE_H
#    define WIDGETCONSOLE_H

#    include "Widget.h"
#    include "Color.h"
#    include "FontRenderer.h"
#    include "Draggable.hpp"

namespace hwidgets
{
	class Console : public Widget
	{
	  public:

		virtual ~Console() { };

		static Console* factory(string& data);

	  protected: // overrides Widget

		long _render(long) override;
		void keyPress(Event& key) override;
		bool script(const string& cmd) override;
		void _help(const string& what) override;
		void mouseClick(Event&) override;

		static void listener(const string& send);

	  private:
		Console();
		Console(const Console&);
		Console& operator =(const Console&);

		string cmd;
		bool redisplay;
		const Color* cmdcolor;
		const Color* anscolor;
		static list<string> display;
	};

}

#endif /* WIDGETCONSOLE_H */

