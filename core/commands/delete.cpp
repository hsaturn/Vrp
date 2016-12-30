/* 
 * File:   svr_delete_cmd.cpp
 * Author: francois
 * 
 */

#include "delete.hpp"
#include <ObjectBuilder.hpp>

using namespace std;

namespace core
{
  delete_cmd delete_cmd_instance;

  delete_cmd::delete_cmd()
  {
	  registerSyntax("delete", "object1 [object2 [...]]", "Delete objects");
  }

  bool delete_cmd::run(Server* psvr, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  bool bRet = true;

	  while (sArgs.length())
	  {
		  string name(StringUtil::getWord(sArgs));
		  if (ObjectBuilder::destroyInstance(name))
			  psvr->send("#OK " + name + " deleted.");
		  else
		  {
			  bRet = false;
			  psvr->send("#KO no object with name " + name);
		  }
	  }

	  return bRet;
  }
}