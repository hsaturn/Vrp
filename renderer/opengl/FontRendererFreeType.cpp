/* 
 * File:   FontRendererFreeType.cpp
 * Author: hsaturn
 * 
 * Created on 2 avril 2019, 02:56
 */


#include <GLShader.hpp>

#include "FontRendererFreeType.h"

#include <unordered_map>
#include <iostream>
#include <freetype2/freetype/ftimage.h>

using namespace std;

static FT_Library ft;
static GLuint shader = 0;
static GLuint vbo;
static GLint attribute_coord;
static GLint uniform_tex;
static GLint uniform_color;

static unordered_map<string, FT_Face> faces;

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

GLint get_attrib(GLuint program, const char *name) {
	GLint attribute = glGetAttribLocation(program, name);
	if(attribute == -1)
		fprintf(stderr, "Could not bind attribute %s\n", name);
	return attribute;
}

GLint get_uniform(GLuint program, const char *name) {
	GLint uniform = glGetUniformLocation(program, name);
	if(uniform == -1)
		fprintf(stderr, "Could not bind uniform %s\n", name);
	return uniform;
}

bool FontRendererFreeType::initFreeType()
{
   static bool bInit=false;
   if (bInit)
   {
      return true;
   }

   if (FT_Init_FreeType(&ft))
   {
      cerr << "ERROR: Could not initialize freetype library" << endl;
      return false;
   }
   else
   {
      cout << "FREETYPE INITIALIZATION OK" << endl;
      bInit = true;
   }
   return true;
}

void FontRendererFreeType::initGl()
{
   shader = GLShader::loadGlsl("text");
   glGenBuffers(1, &vbo);
	attribute_coord = get_attrib(shader, "coord");
	uniform_tex = get_uniform(shader, "tex");
	uniform_color = get_uniform(shader, "color");

	if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
   {
      cerr << "ERROR: FontRenderer unable to initialize opengl" << endl;
   }
   
   cout << "Freetype initGL OK" << endl;

   // TODO when is that shader deallocated ???
}

FT_Face* FontRendererFreeType::loadFont(const string& fontName)
{
   static string defaultFont="data/fonts/FreeSans.ttf";
   
   auto it=faces.find(fontName);
   if (it != faces.end())
   {
      return &(it->second);
   }
   string fontFile = fontPath + fontName+".ttf";
   
   if (FT_New_Face(ft, fontFile.c_str(), 0, &faces[fontName]))
   {
      cerr << "Unable to open font " << fontFile << endl;
      if (FT_New_Face(ft, defaultFont.c_str(), 0, &faces[fontName]))
      {
         auto it=faces.find(fontName);
         faces.erase(it);
         return nullptr;
      }
      else
      {
         cerr << "... loading default font instead." << endl;
      }
   }
   else
   {
      cout << "Font " << fontName << " loaded" << endl;
   }
   return &faces[fontName];
}

FontRenderer* FontRendererFreeType::factory(string& data)
{
   FontRendererFreeType* pRenderer = new FontRendererFreeType; // Mais quelle conception de merde !
   
   initFreeType();
   initGl();
   
   const string fontName(getWord(data));
   const string fontSize(getWord(data));
   
   if (atoi(fontSize.c_str())==0)
   {
      cerr << "Error in Font size (" << fontName << ',' << fontSize << ")" << endl;
   }
   else
   {
      pRenderer->font_height = atoi(fontSize.c_str());
   }
   
   pRenderer->face = loadFont(fontName);
   
   cout << "Loading freetype data (" << data << ")" << endl;
   
   return pRenderer;
}

void FontRendererFreeType::render(int xx, int yy, const string& text) const
{
   if (face == nullptr || shader==0)
   {
      static bool printed=false;
      if (printed==false)
      {
         printed=true;
         cerr << "ERROR: FontRenderer Face or Shader == 0 " << endl;
      }
      return;
   }
   
   glPushMatrix();
   glLoadIdentity();
   glUseProgram(shader);
   
   extern int SCREEN_WIDTH;
   extern int SCREEN_HEIGHT;
   
   float sx= 2.0 / (float)SCREEN_WIDTH;
   float sy = 2.0 / (float)SCREEN_HEIGHT;
   float x=xx * sx -1;
   float y=yy * sy -1;
   
   // TODO VRAC
   glGetError();
   glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   GLfloat white[4] = { 1, 1, 1, 1 };
   FT_Set_Pixel_Sizes(*face, 0, font_height); // TODO
	glUniform4fv(uniform_color, 1, white); // TODO this is the foreground color !
   
   FT_GlyphSlot g = (*face)->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
   glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (auto c : text) {
		/* Try to load and render the character */
		if (FT_Load_Char(*face, c, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
      
		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
   glUseProgram(0);
   glPopMatrix();
   auto er=glGetError();
   if (er)
   {
      cerr << "ERR GL : " << er << endl;
   }
}
