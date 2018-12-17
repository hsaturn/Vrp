/* 
 * File:   FontRendererGlut.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */

#pragma once

#include "FontRenderer.h"
#include <string>
using namespace std;

class FontRendererGlut : public FontRenderer
{
	public:

		static FontRenderer* factory(string& data);

		void render(int x, int y, const string& text) const override;

		virtual int height() const { return font_height; }

	private:
		int font_height;
		void* font;
};

