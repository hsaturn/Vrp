/* 
 * File:   Help.h
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 01:14
 */

#ifndef HELP_H
#define	HELP_H

#include <string>
#include <set>

using namespace std;

class Help {
public:
	typedef set<string> container;
	Help(){};
	
	void add(string s) { m.insert(s); }
	const container& get() const { return m; }
  
private:
	Help(const Help& orig);
	container m;

};

#endif	/* HELP_H */

