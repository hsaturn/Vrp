/* 
 * File:   SmartIndividual.cpp
 * Author: hsaturn
 * 
 * Created on 26 novembre 2015, 20:55
 */

#include <list>

#include "SmartIndividual.hpp"
#include "Random.hpp"
#include "Planet.hpp"
#include "Galaxy.hpp"
#include "GeneLink.hpp"

#include <map>

using namespace std;

SmartIndividual::SmartIndividual()
: Individual() {
}

void SmartIndividual::createGenes(Galaxy* galaxy) {
	Planet* last = 0;
	
	vector<Planet*> planets;
	for (auto planet : *galaxy)
		planets.push_back(planet);

	vector<Planet*>::iterator it;
	it = planets.begin() + Random::rand() % planets.size();
	
	last=*it;

	genes.push_back(shared_ptr<Gene>(new Gene(*it)));
	planets.erase(it);
	
	while(planets.size())
	{
		float min=9e9;
		vector<Planet*>::iterator itMin = planets.end();
		
		for(vector<Planet*>::iterator it=planets.begin(); it!=planets.end(); it++)
		{
			float dist=last->distanceTo(*it);
			if (dist<min)
			{
				min=dist;
				itMin=it;
			}
		}
		if (itMin != planets.end())
		{
			genes.push_back(shared_ptr<Gene>(new Gene(*itMin)));
			last=*itMin;
			planets.erase(itMin);
		}
		else
		{
			cerr <<__FILE__ << ':' << __LINE__ << "?????????????????????????" << endl;
			break;
		}
	}
}