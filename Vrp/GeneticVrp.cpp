/* 
 * File:   GeneticVrp.cpp
 * Author: francois
 * 
 * Created on 24 novembre 2015, 15:44
 */

#include "GeneticVrp.hpp"
#include <iostream>
#include <map>
#include <vector>
#include "Individual.hpp"
#include <Population.hpp>
#include <SmartIndividual.hpp>

using namespace std;

GeneticVrp::~GeneticVrp() { }

GeneticVrp::GeneticVrp(AbstractIndividualFactory* oIndividualFactory,
					   AbstractCrossOver* oCrossOver,
					   AdaptatorFunction* fAdaptator,
					   int populationSize,
					   Galaxy* g
					   )
: AbstractGeneticAlgo(oIndividualFactory, oCrossOver, fAdaptator, populationSize),
	mgalaxy(g)
{
	cout << "NEW GENETIC VRP" << endl;
}

void GeneticVrp::elitism(Population* newPopulation)
{
	multimap<float, Population::type> inds;

	for (auto ai : *actualPopulation)
	{
		shared_ptr<Individual> ind = dynamic_pointer_cast<Individual>(ai);
		inds.insert(pair<float, Population::type>(mfAdaptator->fitnessScore(ind), ai));
	}

	auto it = inds.begin();
	float score = -1;
	unsigned int nr = getActualPopulation()->size() / 10;
	if (nr == 0) nr = 1;
	for (unsigned int i = 0; i < nr;)
	{
		if (it == inds.end()) break;
		{
			if (it->first != score)
			{
				i++;
				score = it->first;
				newPopulation->push_back(it->second);
			}
		}
		it++;
	}
	
	/*for (auto pair : inds)
		cout << "Score : " << pair.first << " : " << pair.second << endl;
	*/

	for (int i = 0; i < 5; i++)
	{
		newPopulation->push_back(moIndividualFactory->build());
		//SmartIndividual* p=new SmartIndividual();
		//p->createGenes(mgalaxy);
		//newPopulation->push_back(shared_ptr<AbstractIndividual>(p));
	}
}


