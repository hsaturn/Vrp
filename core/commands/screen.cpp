/*
 * File:   svr_screen.cpp
 * Author: francois
 *
 */

#include "screen.hpp"
#include "widget.hpp"
#include <GLFW/glfw3.h>
#include <iostream>


using namespace std;
extern GLFWwindow* currentWindow;

namespace core
{
  screen screen_instance;

  screen::screen()
  {
	  registerSyntax("screen", "width height", "Resize screen");
  }

  bool screen::run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  int sizex = StringUtil::getFloat(sArgs);
	  int sizey = StringUtil::getFloat(sArgs);
	  if (sizex < 100) sizex = 100;
	  if (sizey < 100) sizey = 100;
	  if (sizex > 4000) sizex = 4000;
	  if (sizey > 4000) sizey = 4000;

	  glfwSetWindowSize(currentWindow, sizex, sizey);
	  cerr << "Screen resized to " << sizex << " x " << sizey << endl;

	  return true;
  }
}
