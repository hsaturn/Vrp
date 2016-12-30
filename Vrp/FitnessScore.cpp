/* 
 * File:   FitnessScore.cpp
 * Author: hsaturn
 * 
 * Created on 24 novembre 2015, 03:19
 */

#include "FitnessScore.hpp"
#include "Planet.hpp"

FitnessScore::~FitnessScore() {
}

float FitnessScore::fitnessScore(Population::type p) const
{
	float score=0;
	shared_ptr<Individual> ind=dynamic_pointer_cast<Individual>(p);
	if (ind)
	{
		Planet* start=0;
		for (auto gene: *ind)
		{
			
			if (start==0)
				start=gene->getPlanet();
			else
			{
				score += start->distanceTo(gene->getPlanet());
				start=gene->getPlanet();
			}
		}
	}
	return score;
}