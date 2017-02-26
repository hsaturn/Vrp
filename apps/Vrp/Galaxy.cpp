/* 
 * File:   Country.cpp
 * Author: francois
 * 
 * Created on 19 novembre 2015, 14:53
 */

#include "Galaxy.hpp"
#include "Planet.hpp"
#include "Random.hpp"
#include "Ring.hpp"
#include <StringUtil.hpp>
#include <iostream>

using namespace std;

static const float min_planet_ray = 1;
static const float max_planet_ray = 3;

Galaxy::GalaxyBuilder Galaxy::builder;

Galaxy::Galaxy(const string& name, string& incoming) : Object(name) {
	bool bFlat = 0;
	bool bSphere = 0;
	cout << "Planet Incoming = " << incoming << endl;
	if (incoming.substr(0,4)=="flat")
	{
		setVar("type", "flat");
		StringUtil::getWord(incoming);
		bFlat=true;
	}
	else if (incoming.substr(0,6)=="sphere")
	{
		setVar("type","sphere");
		StringUtil::getWord(incoming);
		bSphere=true;
	}
	else
		setVar("type","normal");
	long num_planets = atol(getWord(incoming).c_str());
	if (num_planets == 0)
		num_planets = 20;
	else if (num_planets > 1000)
		num_planets = 1000;

	Range range(100, 100, 100);

	try {
		long n = 0;
		cout << "Creating galaxy " << num_planets << endl;
		long tries;

		while (num_planets) {
			cout << "  Planets left : " << num_planets << endl;
			Planet* t;
			if (bSphere)
			{
				float ray=sqrt(range.x()*range.x()+range.y()*range.y()+range.z()*range.z())/2.0;
				float angle1 = Random::rand(0, M_PI);
				float angle2 = Random::rand(0, 2*M_PI);
				
				Point P(sin(angle1)*cos(angle2)*ray, sin(angle1)*sin(angle2)*ray, cos(angle1)*ray);
				
				t = new Planet(StringUtil::to_string(n), P , min_planet_ray + Random::rand1() * (max_planet_ray - min_planet_ray));
			}
			else
				t = new Planet(StringUtil::to_string(n), range.createRandomPoint(1,1,bFlat ? 0 : 1), min_planet_ray + Random::rand1() * (max_planet_ray - min_planet_ray));
			float dist;
			const Planet* p = nearestPlanet(t->pos(), dist);
			if (p && dist < 3 * max_planet_ray) {
				delete t;
				tries++;
				if (tries > 100) {
					cerr << "  Unable to find a valid location for a new planet." << endl;
					break;
				}
			} else {
				n++;
				tries=0;
				if (Random::rand1() > 0.85)
					t->setRing(new RainbowRing());
				tries = 0;
				mcont.push_back(t);
				num_planets--;
			}
		}
		setVar("size", n);
		
	} catch (...) {
		cerr << "EXCEPTION :-(" << flush << endl;
	}

	cout << "New galaxy created, number of planets : " << mcont.size() << endl;
}

Planet* Galaxy::getRandomPlanet() const {
	if (size())
		return mcont[rand() % size()];
	else
		return 0;
}

const Planet* Galaxy::nearestPlanet(const Point& pos, float& computedDistance) {
	const Planet* nearest = 0;
	computedDistance = 9.99e9;

	for (const Planet* p : mcont) {
		float d = pos.distance(p->pos());
		if (d < computedDistance) {
			nearest = p;
			computedDistance = d;
		}
	}
	return nearest;
}

bool Galaxy::_render(bool resetTimer) {
	bool names=getLong("names");
	int quality(getQuality());
	for (auto planet : * this) {
		planet->render(quality);
		if (names)
			planet->renderName();
	}
	return false;	// TODO ?
}

const Planet* Galaxy::getPlanet(const string& planetName) const
{
	cout << "Searching planet (" << planetName << ")" << endl;
	for(Planet* p: mcont)
		if (p->getName()==planetName)
			return p;
	return 0;
}

Object::ExecResult Galaxy::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue&) {
	if (cmd == "distance") {
		cout << "incoming distance (" << incoming << ")" << endl;
		const Planet* p1 = getPlanet(StringUtil::getWord(incoming));
		const Planet* p2 = getPlanet(StringUtil::getWord(incoming));
		if (p1 && p2) {
			svr->send("#OK " + getName() + " planet " + p1->getName() + " to " + p2->getName() + " = " + StringUtil::to_string(p1->distanceTo(p2)));
		} else
			svr->send("#KO Unexistant planet(s)");
		return EXEC_OK;
	}
	return EXEC_UNKNOWN;
}