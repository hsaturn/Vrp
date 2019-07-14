/*
 * ansi_colors.hpp
 *
 *  Created on: 6 mars 2014
 *      Author: francois
 */

#ifndef ANSI_COLORS_HPP_
#define ANSI_COLORS_HPP_

#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <map>

using namespace std;

// OBSOLETE, PLEASE DONT USE
const string OUT_RESET  		= "\033[0m";
const string OUT_BLACK  		= "\033[30m";      /* Black */
const string OUT_RED    		= "\033[31m";      /* Red */
const string OUT_GREEN  		= "\033[32m";      /* Green */
const string OUT_YELLOW 		= "\033[33m";      /* Yellow */
const string OUT_BLUE   		= "\033[34m";      /* Blue */
const string OUT_MAGENTA		= "\033[35m";      /* Magenta */
const string OUT_CYAN   		= "\033[36m";      /* Cyan */
const string OUT_WHITE  		= "\033[37m";      /* White */
const string OUT_BOLDBLACK		= "\033[1m\033[30m";      /* Bold Black */
const string OUT_BOLDRED   		= "\033[1m\033[31m";      /* Bold Red */
const string OUT_BOLDGREEN 		= "\033[1m\033[32m";      /* Bold Green */
const string OUT_BOLDYELLOW		= "\033[1m\033[33m";      /* Bold Yellow */
const string OUT_BOLDBLUE  		= "\033[1m\033[34m";      /* Bold Blue */
const string OUT_BOLDMAGENTA	= "\033[1m\033[35m";      /* Bold Magenta */
const string OUT_BOLDCYAN  		= "\033[1m\033[36m";      /* Bold Cyan */
const string OUT_BOLDWHITE 		= "\033[1m\033[37m";      /* Bold White */

class Ansi
{
public:
	static const char* reset()		{ return "\033[0m"; }
	static const char* color(unsigned int color)
	{
		static char col[16];
		sprintf(col, "\033[3%cm", color);
		return col;
	}
	static const char* black()		{ return "\033[30m"; }
	static const char* red()		{ return "\033[31m"; }
	static const char* green()		{ return "\033[32m"; }
	static const char* yellow()		{ return "\033[33m"; }
	static const char* blue()		{ return "\033[34m"; }
	static const char* magenta()	{ return "\033[35m"; }
	static const char* cyan()		{ return "\033[36m"; }
	static const char* white()		{ return "\033[37m"; }
	static const char* boldBlack()	{ return "\033[1m\033[30m"; }  // ???
	static const char* boldRed()	{ return "\033[1m\033[31m"; }
	static const char* boldGreen()	{ return "\033[1m\033[32m"; }
	static const char* boldYellow()	{ return "\033[1m\033[33m"; }
	static const char* boldBlue()	{ return "\033[1m\033[34m"; }
	static const char* boldMagenta(){ return "\033[1m\033[35m"; }
	static const char* boldCyan()	{ return "\033[1m\033[36m"; }
	static const char* boldWhite()	{ return "\033[1m\033[37m"; }
	
	static const char* backgroundBlack()		{ return "\033[40m"; }
	static const char* backgroundRed()			{ return "\033[41m"; }
	static const char* backgroundGreen()		{ return "\033[42m"; }
	static const char* backgroundYellow()		{ return "\033[43m"; }
	static const char* backgroundBlue()			{ return "\033[44m"; }
	static const char* backgroundMagenta()		{ return "\033[45m"; }
	static const char* backgroundCyan()			{ return "\033[46m"; }
	static const char* backgroundWhite()		{ return "\033[47m"; }
	static const char* backgroundBoldBlack()	{ return "\033[1m\033[40m"; }  // ???
	static const char* backgroundBoldRed()		{ return "\033[1m\033[41m"; }
	static const char* backgroundBoldGreen()	{ return "\033[1m\033[42m"; }
	static const char* backgroundBoldYellow()	{ return "\033[1m\033[43m"; }
	static const char* backgroundBoldBlue()		{ return "\033[1m\033[44m"; }
	static const char* backgroundBoldMagenta()	{ return "\033[1m\033[45m"; }
	static const char* backgroundBoldCyan()		{ return "\033[1m\033[46m"; }
	static const char* backgroundBoldWhite()	{ return "\033[1m\033[47m"; }
	
	static const char* clrScr()		{ return "\033[2J"; }
	/** clear to end of line
	 */
	static const char* clrToEol()	{ return "\033[K"; }
	static const char* clrToTop()	{ return "\033[1J"; }
	static const char* gotoxy(unsigned int x, unsigned int y)
	{
		static char xy[16];
		sprintf(xy, "\033[%d;%dH",x,y);
		return xy;
	}

	static void getCursorPos(int &x, int &y)
	{
		cout << "\033[6n";
		scanf("\033[%d;%dR", &x, &y);
	}
	static void scrollUp(unsigned int iRows = 1) { cout << "\033[" << iRows << 'S'; }
	static void scrollDown(unsigned int iRows = 1) { cout << "\033[" << iRows << 'T'; }
 	static const char* saveCursor()		{ return "\033[s";		}
	static const char* restoreCursor()	{ return "\033[u";		}
	static const char* hideCursor()		{ return "\033[?25l";	}
	static const char* showCursor()		{ return "\033[?25h";	}
	
	static void getScreenSize(struct winsize &w)
	{
		ioctl(0, TIOCGWINSZ, &w);
	}

	static const char* color(const string sColor)
	{
		static map<string,const char*> colors;
		static const char* none("");
		if (colors.size()==0)
		{
			colors["black"] = black();
			colors["red"] = red();
			colors["yellow"] = yellow();
			colors["blue"] = blue();
			colors["cyan"] = cyan();
			colors["magenta"] = magenta();
			colors["green"] = green();
			colors["white"] = white();

			colors["bold_red"] = boldRed();
			colors["bold_blue"] = boldBlue();
			colors["bold_yellow"] = boldYellow();
			colors["bold_cyan"] = boldCyan();
			colors["bold_magenta"] = boldMagenta();
			colors["bold_green"] = boldGreen();
			colors["bold_white"] = boldWhite();
		}
		if (colors.find(sColor) != colors.end())
			return colors[sColor];
		else
			return none;
	}
};

#endif /* ANSI_COLORS_HPP_ */
