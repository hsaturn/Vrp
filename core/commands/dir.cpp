/* 
 * File:   dir.cpp
 * Author: francois
 * 
 * Created on 8 avril 2016, 16:52
 */

#include "dir.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex.h>
#include "../String.hpp"

dir::dir() { }

dir::dir(const dir& orig) { }

dir::~dir() { }

  int dir::getDir(const string& dir, map<string,string>& files, stringstream& err)
  {
	  regex_t* tpReg = 0;
	  DIR *dp;
	  struct dirent *dirp;
	  
	  string dir2 = dir; //Mask::config().security.Class["Cat"].realname(dir);
	  
	  string::size_type slash = dir2.rfind('/');
	  if (slash!=string::npos)
	  {
			tpReg = new regex_t;
			String regex = dir2.substr(slash+1);
			regex.replace(".","\\.");
			regex.replace("*",".*");
			regex="^"+regex+"$";
			int iResult = regcomp(tpReg, regex.c_str(), REG_NOSUB | REG_EXTENDED);
			if (iResult)
			{
				err << "Bad pattern : " << regex << endl;
				return -1;
			}
			dir2.erase(slash);
	  }

	  if ((dp = opendir(dir2.c_str())) == NULL)
	  {
		  err << "Error(" << errno << ") opening " << dir2 << endl;
		  return -1;
	  }

	  while ((dirp = readdir(dp)) != NULL)
	  {
		  bool bOk = true;
		  if (tpReg)
		  {
			  int match = regexec(tpReg, dirp->d_name, 0, NULL, 0);
			  bOk = match==0;
		  }
		  if (bOk) files[dirp->d_name]="";
	  }
	  closedir(dp);
	  return files.size();
  }
