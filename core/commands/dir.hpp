/* 
 * File:   dir.hpp
 * Author: francois
 *
 * Created on 8 avril 2016, 16:52
 */

#ifndef DIR_HPP
#define	DIR_HPP

#include <string>
#include <map>

using namespace std;

class dir {
public:
	dir();
	dir(const dir& orig);
	virtual ~dir();

	/**
	 * @param dir
	 * @param files result
	 * @return < 0 ; error (acces / pattern) >=0 number of files
	 */
	static int getDir(const string& dir, std::map<string, string> &files, stringstream& err);

private:

};

#endif	/* DIR_HPP */

