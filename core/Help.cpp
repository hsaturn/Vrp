/*
 * File:   Help.cpp
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 01:14
 */

#include "Help.h"
#include <iostream>

using namespace std;

string Help::help_class;

void Help::_pushClass(const string& sclass)
{
   if (sclass.length()==0) return;

   if (help_class.length()) help_class +='.';
   help_class += sclass;
   cout << "Pushing class " << sclass << " : " << help_class << endl;
}

void Help::_popClass()
{
   cout << "poping class (" << help_class << ")" << endl;
   if (help_class.length()==0)
   {
      cerr << "Help::popClass for and empty class" << endl;
      return;
   }
   auto pos=help_class.rfind('.');
   if (pos==string::npos)
      help_class="";
   else
      help_class.erase(pos);
   cout << " = " << help_class << endl;
}