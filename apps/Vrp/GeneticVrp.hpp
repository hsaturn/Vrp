/* 
 * File:   GeneticVrp.hpp
 * Author: francois
 *
 * Created on 24 novembre 2015, 15:44
 */

#ifndef GENETICVRP_HPP
#define	GENETICVRP_HPP

#include <genetic.hpp>

using namespace genetic;

class Galaxy;

class GeneticVrp : public AbstractGeneticAlgo {
public:
	GeneticVrp(AbstractIndividualFactory*,
			AbstractCrossOver*,
			AdaptatorFunction*,
			int populationSize,
			Galaxy*);
	virtual ~GeneticVrp();

	virtual void elitism(Population* newPopulation);

private:
	Galaxy* mgalaxy;

};

#endif	/* GENETICVRP_HPP */

