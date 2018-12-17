/* 
 * File:   FontRenderer.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */

#pragma once

#include <string>
#include <map>

using namespace std;

extern string getWord(string& s, const string &sSeparators = " ");
class FontRenderer
{
	public:

		/**
		 * 
		 * @param data
		 * @return a font renderer (always not null)
		 */
		static FontRenderer* factory(string& data);

		virtual void render(int x, int y, const string& text) const=0;

		virtual int height() const { return font_height; } // TODO ugly
		
	protected:

		int font_height;

	private:
		map<string, FontRenderer*>	mRenderers;
};

