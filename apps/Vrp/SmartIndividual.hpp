/* 
 * File:   SmartIndividual.hpp
 * Author: hsaturn
 *
 * Created on 26 novembre 2015, 20:55
 */

#ifndef SMARTINDIVIDUAL_HPP
#define	SMARTINDIVIDUAL_HPP

#include "Individual.hpp"

class SmartIndividual : public Individual{
public:
	SmartIndividual();
	~SmartIndividual() override {};
	void createGenes(Galaxy*) override;

protected:
	
private:
	SmartIndividual(const SmartIndividual& orig);

};

#endif	/* SMARTINDIVIDUAL_HPP */

