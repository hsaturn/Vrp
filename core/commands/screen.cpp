/* 
 * File:   svr_screen.cpp
 * Author: francois
 * 
 */

#include "screen.hpp"
#include <GL/glut.h>

using namespace std;

namespace core
{
  screen screen_instance;

  screen::screen()
  {
	  registerSyntax("screen", "width height", "Resize screen");
  }

  bool screen::run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  bool bRet = false;

	  int sizex = StringUtil::getFloat(sArgs);
	  int sizey = StringUtil::getFloat(sArgs);
	  if (sizex < 100) sizex = 100;
	  if (sizey < 100) sizey = 100;
	  if (sizex > 4000) sizex = 4000;
	  if (sizey > 4000) sizey = 4000;
	  glutReshapeWindow(sizex, sizey);

	  return bRet;
  }
}