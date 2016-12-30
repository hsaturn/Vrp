/* 
 * File:   FontRenderer.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */

#ifndef FONTRENDERER_H
#define	FONTRENDERER_H

#include <string>
using namespace std;

class FontRenderer
{
  public:
	
	/**
	 * 
     * @param data
     * @return a font renderer (always not null)
     */
	static FontRenderer* factory(string& data);
	
	void render(int x, int y, const string& text);
	
	int height() { return font_height; }
	
  private:
	void* font;
	int font_height;
};

#endif	/* FONTRENDERER_H */

