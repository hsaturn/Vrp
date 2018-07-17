/* 
 * File:   IndividualFactory.cpp
 * Author: hsaturn
 * 
 * Created on 23 novembre 2015, 19:51
 */

#include "IndividualFactory.hpp"
#include "ObjectBuilder.hpp"
#include "Galaxy.hpp"
#include "Individual.hpp"
#include "SmartIndividual.hpp"
#include <StringUtil.hpp>

IndividualFactory::IndividualFactory(
	string galaxyName, string params)
: mgalaxyName(galaxyName)
{
	cout << "Params = " << params << endl;
	indType="random";
	while(params.length())
	{
		string param=StringUtil::getWord(params);
		if (param=="smart")
			indType="smart";
	}
	cout << "IND TYPE=" << indType << endl;
}

IndividualFactory::IndividualFactory(const IndividualFactory& orig) {
}

IndividualFactory::~IndividualFactory() {
}

Galaxy* IndividualFactory::galaxy() const {
	return dynamic_cast<Galaxy*> (ApplicationBuilder::getInstance(mgalaxyName));
}

Population::type IndividualFactory::build() {
	shared_ptr<Individual> ind;
	if (indType=="smart")
		ind=shared_ptr<SmartIndividual>(new SmartIndividual());
	else
	{
		ind=shared_ptr<Individual>(new Individual());
	}
	
	ind->createGenes(galaxy());
	return ind;
}