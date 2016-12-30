/* 
 * File:   Help.h
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 01:14
 */

#ifndef HELP_H
#define	HELP_H

#include <string>
#include <map>

using namespace std;

class Help {
public:
	typedef map<string,int> container;
	Help(){};
	
	void add(string s) { m[s]=1; }
	const container& get() const { return m; }
private:
	Help(const Help& orig);
	container m;

};

#endif	/* HELP_H */

