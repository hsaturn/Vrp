/* 
 * File:   FontRenderer.cpp
 * Author: francois
 * 
 * Created on 7 juillet 2015, 17:42
 */
#include <functional>

#include "Color.h"
#include <GL/freeglut_std.h>

#include "FontRenderer.h"
#include "FontRendererGlfw.h"
#include "FontRendererFreeType.h"
#include "core/StringUtil.hpp"

string FontRenderer::fontPath = "data/fonts/";
map<size_t, const FontRenderer*> FontRendererFactory::renderers;
map<string, const FontRenderer*> FontRendererFactory::renderers_per_name;
   
void FontRenderer::setFontPath(string& data)
{
   if (data.length() && data[data.length()-1]!='/')
      data += '/';
   fontPath = data;
   data="";
}

size_t FontRenderer::getHash(size_t prev) const
{
   size_t h1 = hash<string>()(fontPath);
   size_t h2 = hash<int>()(font_height);
   
   return h1 ^ (h2 << 1) ^ (prev << 2);
}

// Builds a font
// string="[name] type specific_data"
// type=either ft or bitmap (bad design but this is like that
const FontRenderer* FontRendererFactory::build(string& data)
{
   const FontRenderer* pRenderer{nullptr};
   
   string font_name(StringUtil::getWord(data));
   string font_renderer_type;
   
   if (font_name == "ft" || font_name == "freetype" || font_name == "bitmap")
   {
      font_renderer_type = font_name;
      font_name = "";
   }
   
   if (font_name.length() != 0)
   {
      auto it_per_name = renderers_per_name.find(font_name);
      
      if (it_per_name != renderers_per_name.end())
      {
         pRenderer = it_per_name->second;
         return pRenderer;
      }
      else
      {
         // font not found by name, expect definition after the name
         font_renderer_type = StringUtil::getWord(data);
      }
   }
   
   if (font_renderer_type == "ft" ||
       font_renderer_type == "freetype")
   {
      pRenderer = FontRendererFreeType::factory(data);
   }
   else if (font_renderer_type == "bitmap")
   {
      pRenderer = FontRendererGlfw::factory(data);
   }
   else
   {
      cerr << "ERROR: Unknown font renderer type (" << font_renderer_type << ")" << endl;
      return nullptr;   // What happen then ???
   }
   
   // delete if already exists
   if (pRenderer)
   {
      size_t h = pRenderer->getHash();
      auto it=renderers.find(h);
      if (it != renderers.end())
      {
         delete pRenderer;
         pRenderer = it->second;
         cout << "Deleting temporary font (duplicate)" << endl;
      }
      else
      {
         renderers[h] = pRenderer;
      }
      if (font_name.length())
      {
         renderers_per_name[font_name] = pRenderer;
         cout << "FontRendererFactory: registered new font [" << font_name << "]" << endl;
      }
   }
   return pRenderer;
}

