/* 
 * File:   WidgetConsole.cpp
 * Author: francois
 * 
 * Created on 7 juillet 2015, 13:34
 */

#include "Console.h"
#include "Server.h"
#include <sys/socket.h>
#include "ClipBoard.h"
#include "Draggable.hpp"
#include "Event.hpp"
#include "WidRect.hpp"

namespace hwidgets
{
	WIDGET_REGISTER("console", Console);
	
	static bool bEvents = true;
	static bool bErrors = true;
	static bool bCommands = true;
	static bool bStack = true;
	static bool bHidden = false;

	list<string> Console::display;
	
	Console::Console()	{}

	void Console::listener(const string& send)
	{
		static string last = "";
		if (true) //send != last)
		{
			last = send;
			while (display.size() > 80)
				display.erase(display.begin());
			display.push_back(send);
		}
	}

	void Console::mouseClick(Event &event)
	{
		;
		if (event.mouse.buttons.middle)
		{
			string s = ClipBoard::get();
			Event key;
			key.type = Event::EVT_KEYBD_DOWN;
			key.mouse.x = 0;
			key.mouse.y = 0;
			while (s.length())
			{
				key.key = (Event::Key)s[0];
				s.erase(0, 1);
			}
		}
	}

	void Console::_help(const string& what)
	{
		pushHelp("console [error|commands|events|stack|hidden] [on|off]");
	}

	bool Console::script(const string& cmd)
	{
		if (cmd[0] != '@')
		{
			listener("> " + cmd);
		}
		string s(cmd);
		while (s[0] == '@') s.erase(0, 1);
		string s1 = getWord(s);
		if (s1 == "console")
		{
			s1 = getWord(s);
			string sw = getWord(s);
			bool* b = 0;
			if (s1 == "events") b = &bEvents;
			else if (s1 == "commands") b = &bCommands;
			else if (s1 == "errors") b = &bErrors;
			else if (s1 == "stack") b = &bStack;
			else if (s1 == "hidden") b = &bHidden;
			else return false;

			if (sw == "on")		*b = true;
			else if (sw == "off")	*b = false;
			return true;
		}
		else if (s1 == "clear")
		{
			display.clear();
			return true;
		}
		return false;
	}

	Console* Console::factory(string& data)
	{
		static bool listener = false;

		if (listener == false)
		{
			Server::addListener(Console::listener);
		}
		Console* cons = 0;
		if (data.substr(0, 4) == "help")
		{
			Widget::pushMessage("console coord coord coord coord [bgcolor cmdcolor color eventcolor] [font_family font_size]");
		}
		else
		{
			static int n = 0;
			cons = new Console;
			cons->setName("console #" + to_string((long long) (++n)));
			cons->cmdcolor = Color::factory(data);
			cons->anscolor = Color::factory(data);
			cons->font = FontRenderer::factory(data);
		}
		return cons;
	}

	void Console::keyPress(Event& keyevent)
	{
		redisplay = true;
		uint16_t key = keyevent.key;
		
		switch (key)
		{
			case 27:
				exit(0);

			case 8: // BACK
				if (cmd.size())
					cmd.erase(cmd.size() - 1, 1);
				break;

			case 9:	// TAB
				break;

			case 13:
				if (cmdQueue)
					cmdQueue->push_back(cmd);
				cmd = "";
				break;

			default:
				if (key >= 32 && key<256)
				{
					cmd += key;
				}
				else
					cout << " (" << (int) key << ") ";
				break;

		}

		// TODO glutPostRedisplay();
		setVar("_cmdline=\"" + cmd + "\"");
		cout << "CUBE_SERVER> " << cmd << "                      \r" << flush;
	}

	long Console::_render(long time)
	{
		string cmdt("> " + cmd);

		if ((time % 500) > 250) cmdt += '_';

		float x = rect()->x1() + 2;
		float y = rect()->y2() - 6;

		if (cmdcolor)
		{
			cmdcolor->render();
		}
		else
			Color::white.render();

		glTranslatef(0.0, 0.0, 1.0);


		font->render(x, y, cmdt);
		y -= font->height();

		if (bStack && cmdQueue)
		{
			if (cmdcolor)
				cmdcolor->render(0.5);
			else
				Color::white.render(0.8);
			auto its = cmdQueue->rbegin();
			while (y > rect()->y1() + 10 && its != cmdQueue->rend())
			{
				string cmd = *its;
				its++;
				if (bHidden || cmd[0] != '@')
				{
					cmd = "(pending) " + cmd;

					font->render(x, y, cmd);

					y -= font->height();
				}
			}
		}

		auto itd = display.rbegin();

		while (y > rect()->y1() + 10 && itd != display.rend())
		{
			string cmdt = *itd;
			itd++;

			if (cmdt[0] == '>')
			{
				if (bCommands == false) continue;
				if (cmdcolor)
					cmdcolor->render();
				else
					Color::white.render(0.8);
			}
			else if (cmdt.substr(0, 3) == "#KO" || cmdt.substr(0, 6) == "#ERROR")
			{
				if (bErrors == false) continue;
				Color::red.render(0.8);
			}
			else if (cmdt.substr(0, 3) == "#OK")
			{
				Color::green.render(0.8);
			}
			else if (cmdt.substr(0, 6) == "#EVENT")
			{
				if (bEvents == false) continue;
				Color::cyan.render(0.8);
			}
			else
			{
				if (anscolor)
					anscolor->render();
				else
					Color::cyan.render(0.8);
			}
			font->render(x, y, cmdt);

			y -= font->height();
		}
		glTranslatef(0.0, 0.0, -1.0);

		return 200;
	}
}
