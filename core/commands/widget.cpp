/* 
 * File:   svr_widget.cpp
 * Author: francois
 * 
 */

#include "widget.hpp"
#include <Widget.h>

using namespace std;

namespace core
{
static widget widget_instance;

widget::widget()
{
   registerSyntax("widget", "", "Commande de widget");
}

bool widget::run(Server* server, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
{

   try
   {
      hwidgets::Widget* widget = hwidgets::Widget::factory(sArgs);
      if (widget)
         server->send("#OK widget");
      else
         server->send("#KO widget");
   }
   catch (const char* p)
   {
      cerr << "CATCH " << p << ", Command=" << sCmd << " ... " << sArgs << endl;
      server->send(string("#KO widget ") + p);
   }
   catch (...)
   {
      cerr << "CATCH DURING WIDGET::FACTORY" << endl;
      server->send("#KO widget exception");
   }

   return false;  // TODO What else ?
}
}