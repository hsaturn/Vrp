/* 
 * File:   svr_new_cmd.cpp
 * Author: francois
 * 
 */

#include "new.hpp"
#include <Object.h>

using namespace std;

namespace core
{
  new_cmd new_cmd_instance;

  new_cmd::new_cmd()
  {
	  registerSyntax("new", "object [object_definition]", "Create a new object");
  }

  bool new_cmd::run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  bool bRet = false;
	  
	  string obj_class = StringUtil::getWord(sArgs);
	  Object* object = ObjectBuilder::buildInstance(obj_class, sArgs);

	  if (object)
	  {
		  bRet = true;
		  psvr->send("#OK new " + obj_class + " : " + object->getName());
	  }
	  else
		  psvr->send("#KO new " + obj_class);

	  return bRet;
  }
}