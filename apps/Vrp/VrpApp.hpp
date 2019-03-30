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

class VrpApp : public Application {

	APPLICATION_BUILDER("vrp", VrpApp);
	
public:
	VrpApp(const string& name, string &incoming);

	~VrpApp() override {}

	bool _render(bool resetTimer) override;
	Application::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
	void _help(Help&) override;
	void best(Server*);
	void pop();
	
private:
	Galaxy* galaxy();

	bool solve();
	void reset();

	VrpApp(const VrpApp& orig);

	string mgalaxyName;
	Galaxy* galaxyPtr;	// Detect ptr modification
	AbstractGeneticAlgo* algo;
	string solveParams;
};

#endif	/* VRP_HPP */

