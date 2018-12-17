/* 
 * File:   FontRendererGlfw.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */

#pragma once

#include "FontRenderer.h"
#include <string>
using namespace std;

class FontRendererGlfw : public FontRenderer
{
	public:
		static FontRenderer* factory(string& data);

		virtual void render(int x, int y, const string& text) const override;

		virtual int height() const { return font_height; }

	private:
		void* font;
		int font_height;
};

