/*
 * File:   FontRenderer.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */
#pragma once

#include "Object.h"
#include <string>
#include <map>

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

		virtual void render(int x, int y, const string& text) const=0;

		virtual int height() const { return font_height; } // TODO ugly

      static void setFontPath(string& data);
      
	protected:

		int font_height;
      static string fontPath;

	private:
		map<string, FontRenderer*>	mRenderers;
};

