/* 
 * File:   svr_font.cpp
 * Author: hsaturn 20191022
 */

#include "font.hpp"
#include <FontRenderer.h>

using namespace std;

namespace core
{
static font font_instance;

font::font()
{
   registerSyntax("font", "name [ft|freetype|bitmap] family height", "Font definition");
}

bool font::run(Server* server, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
{
   bool bRet = false;

   try
   {
      const FontRenderer* pFont = FontRendererFactory::build(sArgs);
      if (pFont)
         server->send("#OK font");
      else
         server->send("#KO font");
   }
   catch (const char* p)
   {
      cerr << "CATCH " << p << ", Command=" << sCmd << " ... " << sArgs << endl;
      server->send(string("#KO font ") + p);
   }
   catch (...)
   {
      cerr << "CATCH DURING FONT::BUILD" << endl;
      server->send("#KO font exception");
   }

   return bRet;
}
}