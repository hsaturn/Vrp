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

class FontRenderer
{
 public:
   virtual ~FontRenderer() = default;

   virtual void render(int x, int y, const string& text) const = 0;

   virtual int height() const { return font_height; } // TODO ugly

   static void setFontPath(string& data);

 protected:

   int font_height;
   static string fontPath;
   
   virtual size_t getHash(size_t prevHash=0) const;

   friend class FontRendererFactory;
};

class FontRendererFactory
{
   public:
     FontRendererFactory() = delete;
     
      /**
       * @param data
       * @return a font renderer or null
       */
      static const FontRenderer* build(string& data);

   private:
      // For flyweight pattern
      static map<size_t, const FontRenderer*> renderers;
      static map<string, const FontRenderer*> renderers_per_name;
};

