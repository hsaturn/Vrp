/* 
 * File:   FontRenderer.cpp
 * Author: francois
 * 
 * Created on 7 juillet 2015, 17:42
 */

#include <GL/freeglut_std.h>

#include "FontRenderer.h"
#include "Color.h"

extern string getWord(string& s, const string &sSeparators = " ");
FontRenderer* FontRenderer::factory(string& data)
{
	FontRenderer* pFont=new FontRenderer;
	
	//cout << "FONT FACTORY " << data << endl;
	
	pFont->font=0;
		
	string data2(data);
	string font = getWord(data2);
	string size = getWord(data2);

	if (font=="helvetica")
	{
		pFont->font=GLUT_BITMAP_HELVETICA_12;
		pFont->font_height=13;
		if (size=="10")
		{
			pFont->font=GLUT_BITMAP_HELVETICA_10;
			pFont->font_height=11;
		}
		else if (size=="18")
		{
			pFont->font=GLUT_BITMAP_HELVETICA_18;
			pFont->font_height=19;
		}
	}
	else if (font=="fixed")
	{
		if (size=="15")
		{
			pFont->font=GLUT_BITMAP_9_BY_15;
			pFont->font_height=15;
		}
	}
	else if (font=="roman")
	{
		if (size=="mono")
			pFont->font=GLUT_STROKE_MONO_ROMAN;
		else
		{
			pFont->font=GLUT_STROKE_ROMAN;
			data2=size+" "+data2;
		}
	}
	else if (font=="times")
	{
		pFont->font=GLUT_BITMAP_TIMES_ROMAN_10;
		pFont->font_height = 10;
		
		if (size=="24")
		{
			pFont->font=GLUT_BITMAP_TIMES_ROMAN_24;
			pFont->font_height=24;
		}
	}
	//cout << endl;
	//cout << "FONT DATA  " << data << " font=" << font << " size=" << size << endl;
	//cout << "FONT DATA2 " << data2 << endl;
		if (pFont->font == 0)
	{
		// Default font
		pFont->font=GLUT_BITMAP_8_BY_13;
		pFont->font_height=13;
	}
	else
	{
		data=data2;
	}
	
	return pFont;
}

void FontRenderer::render(int x, int y, const string& text)
{
	glRasterPos2f(x,y);
	for(auto it=text.begin(); it!=text.end(); it++)
		glutBitmapCharacter(font,*it);
}
