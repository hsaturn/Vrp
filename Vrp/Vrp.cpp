/* 
 * File:   Vrp.cpp
 * Author: francois
 * 
 * Created on 20 novembre 2015, 15:23
 */

#include "Vrp.hpp"

#include <iostream>
#include "Galaxy.hpp"
#include "IndividualFactory.hpp"
#include "Individual.hpp"
#include "FitnessScore.hpp"
#include "GeneticVrp.hpp"
#include "CrossOver.hpp"
#include "SmartIndividual.hpp"
#include <StringUtil.hpp>
#include <GL/glew.h>
#include <sstream>
#include <unistd.h>

using namespace std;

Vrp::VrpBuilder Vrp::builder;

Vrp::Vrp(const string& name, string& incoming)
: Object(name),
algo(0)
{
	setVar("render_best", "1");
	mgalaxyName = StringUtil::getWord(incoming);
	solveParams = incoming;
	cout << "New vrp, galaxy = " << mgalaxyName << endl;
}

Galaxy* Vrp::galaxy()
{
	Galaxy* g = dynamic_cast<Galaxy*> (ObjectBuilder::getInstance(mgalaxyName));
	if (g == 0)
		galaxyPtr = 0;
	return g;
}

void Vrp::_help(Help& help)
{
	help.add("vrp.solve    Search best path");
	help.add("vrp.reset    Delete genetic algo");
	help.add("vrp.best     Display best result");
	help.add("vrp.pause=n");
	help.add("vrp vars : fast");
}

bool Vrp::_execute(Server* server, string cmd, string incoming, const string& org)
{
	bool bRet = true;
	if (cmd == "solve")
	{
		cout << "SOLVE " << cmd << '/' << incoming << '/' << org << endl;
		if (solve())
			server->send("#OK Solving...");
		else
			server->send("#KO No galaxy with name " + mgalaxyName + " exists.");
	}
	else if (cmd == "reset")
		reset();
	else if (cmd == "best")
		best(server);
	else if (cmd == "pop")
		pop();
	else if (cmd == "run")
		setVar("pause", "0");
	else if (cmd == "pause")
		setVar("pause", "1");
	else
		bRet = false;
	return bRet;
}

void Vrp::best(Server* server)
{
	if (algo)
	{
		stringstream s;
		s << getName();
		if (galaxy())
			s << " on " << galaxy()->getName() << " ";
		s << ", best=" << algo->getBestFitness() << ", found at iteration " << algo->getBestPopulation() << " ";

		algo->getBest()->dump(s);
		server->send(s.str());

	}
	else
		server->send("#KO Not solving...");
}

void Vrp::pop()
{
	if (algo)
	{
		algo->pop(cout);
	}
}

void Vrp::reset()
{
	if (algo)
	{
		delete algo;
		algo = 0;
	}
}

bool Vrp::solve()
{
	cout << "SOLVING " << solveParams << endl;
	if (galaxy())
	{
		string params(solveParams);
		string slong(params);
		int popsize = 0;
		while (slong.length())
		{
			popsize = atol(StringUtil::getWord(slong).c_str());
			if (popsize) break;
		}
		if (popsize == 0) popsize = 20;
		galaxyPtr = galaxy();
		reset();
		IndividualFactory* factory = new IndividualFactory(mgalaxyName, solveParams);
		FitnessScore* fitness = new FitnessScore();
		CrossOver* cross = new CrossOver();
		algo = new GeneticVrp(factory, cross, fitness, popsize, galaxy());
		return true;
	}
	else
		return false;

}

extern bool redisplayAsked;
extern Server* server;

void Vrp::_render(bool resetTimer)
{
	if (algo && galaxy())
	{
		if (galaxyPtr != galaxy())
		{
			cout << "GALAXY HAS CHANGED, STARTING A NEW SOLVER" << endl;
			solve();
		}
		if (algo)
		{
			galaxyPtr->glSetMatrix();
			if (getLong("pause") == 0)
			{
				cout << "----------------" << endl;
				if (algo->step())	// FIXME compute while rendering is bad !
					best(server);
				if (getLong("fast"))
					redisplayAsked = true;
			}

			if (getLong("render_best"))
			{
				glPushMatrix();
				algo->getBest()->render(galaxyPtr->getLong("quality"));
				glPopMatrix();
			}
		}
	}
	if (galaxy())
	{
		//static SmartIndividual ind(galaxy());
		//ind.render(10);
	}
}
