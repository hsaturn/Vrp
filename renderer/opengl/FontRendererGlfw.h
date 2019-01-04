/*
 * File:   FontRendererGlfw.h
 * Author: francois
 *
 * Created on 7 juillet 2015, 17:42
 */

#pragma once

#include "FontRenderer.h"
#include "GLShader.hpp"

#include <glm/glm.hpp>
#include <string>
#include <map>

using namespace std;

class FontRendererGlfw : public FontRenderer
{
	class Character
	{
		GLuint textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
	};

	public:
		static FontRenderer* factory(string& data);

		virtual void render(int x, int y, const string& text) const override;

		virtual int height() const { return font_height; }

	private:
		void* font;
		int font_height;
		map<char, Character> mCharacters;
		GLShader shader;
		GLuint VAO, VBO;
};

