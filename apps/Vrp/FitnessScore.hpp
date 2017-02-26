/* 
 * File:   FitnessScore.hpp
 * Author: hsaturn
 *
 * Created on 24 novembre 2015, 03:19
 */

#ifndef FITNESSSCORE_HPP
#define	FITNESSSCORE_HPP

#include <genetic.hpp>
#include <Individual.hpp>

using namespace genetic;

class Galaxy;

class FitnessScore : public AdaptatorFunction {
public:
	FitnessScore(){};
	virtual ~FitnessScore();
	
	virtual float fitnessScore(Population::type) const;

private:
	FitnessScore(const FitnessScore& orig);
};

#endif	/* FITNESSSCORE_HPP */

