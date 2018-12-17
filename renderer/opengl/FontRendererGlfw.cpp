/* 
 * File:   FontRenderer.cpp
 * Author: francois
 * 
 */

#include "Color.h"
#include "FontRendererGlfw.h"
FontRenderer* FontRendererGlfw::factory(string& data)
{
	FontRendererGlfw* pFont=new FontRendererGlfw;
	
	pFont->font=0;
		
	string data2(data);
	string font = getWord(data2);
	string size = getWord(data2);

	if (font=="helvetica")
	{
		//	pFont->font=GLUT_BITMAP_HELVETICA_12;
		pFont->font_height=13;
		if (size=="10")
		{
		// TODO 	pFont->font=GLUT_BITMAP_HELVETICA_10;
			pFont->font_height=11;
		}
		else if (size=="18")
		{
		// TODO	pFont->font=GLUT_BITMAP_HELVETICA_18;
			pFont->font_height=19;
		}
	}
	else if (font=="fixed")
	{
		if (size=="15")
		{
		// TODO	pFont->font=GLUT_BITMAP_9_BY_15;
			pFont->font_height=15;
		}
	}
	else if (font=="roman")
	{
		if (size=="mono")
		{
			// TODO pFont->font=GLUT_STROKE_MONO_ROMAN;
		}
		else
		{
			// TODO pFont->font=GLUT_STROKE_ROMAN;
			data2=size+" "+data2;
		}
	}
	else if (font=="times")
	{
		// TODO pFont->font=GLUT_BITMAP_TIMES_ROMAN_10;
		pFont->font_height = 10;
		
		if (size=="24")
		{
			// TODO pFont->font=GLUT_BITMAP_TIMES_ROMAN_24;
			pFont->font_height=24;
		}
	}
		
		if (pFont->font == 0)
	{
		// Default font
		// pFont->font=GLUT_BITMAP_8_BY_13;
		pFont->font_height=13;
	}
	else
	{
		data=data2;
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
