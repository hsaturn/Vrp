/* 
 * File:   FontRendererFreeType.h
 * Author: hsaturn
 *
 * Created on 2 avril 2019, 02:56
 */
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FontRenderer.h"

class FontRendererFreeType : public FontRenderer
{
   public:
      FontRendererFreeType();
      virtual ~FontRendererFreeType() = default;
      
      void render(int x, int y, const string& text) const override;
      
		static FontRenderer* factory(string& data);

   private:
      static bool initFreeType();
      static void initGl();
      static FT_Face* loadFont(const string& fontName);
      
      FT_Face* face = nullptr;
};

