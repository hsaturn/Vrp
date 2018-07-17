/* 
 * File:   svr_objects.cpp
 * Author: francois
 * 
 */

#include "objects.hpp"
#include <ObjectBuilder.hpp>

using namespace std;

namespace core
{
  objects objects_instance;

  objects::objects()
  {
	  registerSyntax("objects", "", "List all objects");
  }

  bool objects::run(Server* server, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  server->send(ApplicationBuilder::listAll());

	  return true;
  }
}