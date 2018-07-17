/* 
 * File:   IndividualFactory.hpp
 * Author: hsaturn
 *
 * Created on 23 novembre 2015, 19:51
 */

#ifndef INDIVIDUALFACTORY_HPP
#define	INDIVIDUALFACTORY_HPP

#include <genetic.hpp>
#include <string>

using namespace std;
using namespace genetic;

class GeneLink;
class Galaxy;

class IndividualFactory : public AbstractIndividualFactory {
public:
	IndividualFactory(string galaxyName, string params);
	IndividualFactory(const IndividualFactory& orig);
	~IndividualFactory() override;

	Population::type build() override;
	Galaxy* galaxy() const;
	
private:
	const string mgalaxyName;
	string indType;	// Type d'individu créés
};

#endif	/* INDIVIDUALFACTORY_HPP */

