/* 
 * File:   GeneLink.hpp
 * Author: hsaturn
 *
 * Created on 23 novembre 2015, 19:46
 */

#ifndef GENELINK_HPP
#define	GENELINK_HPP

#include <genetic.hpp>

using namespace std;
using namespace genetic;

class Planet;

class GeneLink : public AbstractGene {
public:
	GeneLink();
	GeneLink(const GeneLink& orig);
	~GeneLink() override;
	
	void render() override;

private:
	Planet* planetStart;
	Planet* planetEnd;

};

#endif	/* GENELINK_HPP */

