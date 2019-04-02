/* 
 * File:   FontRenderer.cpp
 * Author: francois
 * 
 * Created on 7 juillet 2015, 17:42
 */

#include "Color.h"
#include <GL/freeglut_std.h>

#include "FontRenderer.h"
#include "FontRendererGlfw.h"
#include "FontRendererFreeType.h"

FontRenderer* FontRenderer::factory(string& data)
{
   const string font_renderer_type(getWord(data));
   
   if (font_renderer_type == "ft" ||
       font_renderer_type == "freetype")
   {
      return FontRendererFreeType::factory(data);
   }
   else if (font_renderer_type == "bitmap")
   {
      return FontRendererGlfw::factory(data);
   }
   else
   {
      cerr << "ERROR: Unknown font renderer type (" << font_renderer_type << ")" << endl;
      return nullptr;   // What happen then ???
   }
}

