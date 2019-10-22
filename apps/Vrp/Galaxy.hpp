/* 
 * File:   Country.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 14:53
 */
#pragma once

#include <vector>
#include "Range.hpp"
#include <ObjectBuilder.hpp>

using namespace std;

class Planet;

class Galaxy : public Application {

	APPLICATION_BUILDER("galaxy", Galaxy);
	
public:
	Galaxy(const string& name, string& incoming);

	Planet* getRandomPlanet() const;
	typedef vector<Planet*> container;
	typedef container::const_iterator const_iterator;
	typedef container::iterator iterator;

	const_iterator begin() const {
		return planets.begin();
	}

	const_iterator end() const {
		return planets.end();
	}

	const Planet* nearestPlanet(const Point&, float& computedDistance);
	const Planet* getPlanet(const string& planetName) const;
	
	~Galaxy() override {};
	
	int size() const { return planets.size(); }
	
	ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
   void _help(Help&) const override;


protected:
	bool _render(bool resetTimer) override;

private:
	container planets;
};
