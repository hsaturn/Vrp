/* 
 * File:   GeneticVrp.hpp
 * Author: francois
 *
 * Created on 24 novembre 2015, 15:44
 */

#pragma once

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
	~GeneticVrp() override;

	void elitism(Population* newPopulation) override;

private:
	Galaxy* mgalaxy;

};
