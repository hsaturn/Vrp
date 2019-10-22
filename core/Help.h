/* 
 * File:   Help.h
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 01:14
 */

#ifndef HELP_H
#define	HELP_H

#include <string>
#include <iostream>
#include <set>

using namespace std;

class Help {
public:
   class PushClass
   {
      public:
         ~PushClass()
         { 
            help._popClass();
         }
      private:
         friend class Help;
         PushClass(Help& h, const string& sClass) : help(h)
         {
            help._pushClass(sClass);
         }
         
         Help& help;
   };

public:
	typedef set<string> container;
	Help(){};
	
	void add(string s) { cout << "adding " << help_class << "(" << s << ")" << endl; m.insert((help_class.length() ? help_class +'.' : string("")) + s); }
	const container& get() const { return m; }
   
   static const string& getClass() { return help_class; }
   [[nodiscard]] PushClass pushClass(const string& name)
   {
      return PushClass(*this, name);
   }
  
private:
   friend class HelpPushClass;
   static void _pushClass(const string& name);
   static void _popClass();
	Help(const Help& orig);
	container m;
   static string help_class;
};

#endif	/* HELP_H */

