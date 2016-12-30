/* 
 * File:   svr_test.cpp
 * Author: francois
 * 
 */

#include "test.hpp"

using namespace std;

namespace core
{
  test test_instance;

  test::test()
  {
	  registerSyntax("test", "", "Commande de test");
  }

  bool test::run(Server* server, string &sCmd, string& sArgs, stringstream& out, stringstream& err)
  {
	  bool bRet = false;
	  
	  server->send("TESSSSSSSSSSSSSSSSSSSSSSSSSST");

	  return bRet;
  }
}