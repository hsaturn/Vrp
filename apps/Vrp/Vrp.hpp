/* 
 * File:   Vrp.hpp
 * Author: francois
 *
 * Created on 20 novembre 2015, 15:23
 */

#ifndef VRP_HPP
#define	VRP_HPP

#include <Object.h>
#include <ObjectBuilder.hpp>
#include <string>
#include "genetic.hpp"

using namespace std;
using namespace genetic;

class Galaxy;

class Vrp : public Application {

	APPLICATION_BUILDER("vrp", Vrp);
	
public:
	Vrp(const string& name, string &incoming);

	virtual ~Vrp() {
	};

	virtual bool _render(bool resetTimer);
	virtual Application::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
	virtual void _help(Help&);
	void best(Server*);
	void pop();
	
private:
	Galaxy* galaxy();

	bool solve();
	void reset();

	Vrp(const Vrp& orig);

	static VrpBuilder builder;

	string mgalaxyName;
	Galaxy* galaxyPtr;	// Detect ptr modification
	AbstractGeneticAlgo* algo;
	string solveParams;
};

#endif	/* VRP_HPP */

