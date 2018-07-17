/* 
 * File:   Country.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 14:53
 */

#ifndef GALAXY_HPP
#define	GALAXY_HPP

#include <vector>
#include "Range.hpp"
#include <Object.h>
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
		return mcont.begin();
	}

	const_iterator end() const {
		return mcont.end();
	}

	const Planet* nearestPlanet(const Point&, float& computedDistance);
	const Planet* getPlanet(const string& planetName) const;
	
	virtual ~Galaxy() {};
	
	int size() const { return mcont.size(); }
	
	virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);

protected:
	virtual bool _render(bool resetTimer);

private:
	container mcont;
	static GalaxyBuilder builder;
};

#endif	/* GALAXY_HPP */

