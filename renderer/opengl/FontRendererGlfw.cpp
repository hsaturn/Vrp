/*
 * File:   FontRenderer.cpp
 * Author: francois
 *
 */

#include <iostream>
#include "Color.h"
#include "FontRendererGlfw.h"
#include <freetype2/ft2build.h>
#include "core/StringUtil.hpp"

FontRenderer* FontRendererGlfw::factory(string& data)
{
	FontRendererGlfw* pFont=new FontRendererGlfw;

	pFont->font=0;

	string data2(data);
   const string font_type = StringUtil::getWord(data2);
	const string font = StringUtil::getWord(data2);
	const string size = StringUtil::getWord(data2);
   
   if (font_type != "bitmap")
   {
      cerr << "Error, only bitmap fonts are supported (" << font_type << " unsupported)" << endl;
   }
   
	return pFont;
}

void FontRendererGlfw::render(int x, int y, const string& text) const
{
	glRasterPos2f(x,y);
	for(auto it=text.begin(); it!=text.end(); it++)
	{
		// glutBitmapCharacter(font,*it);
	}
}
