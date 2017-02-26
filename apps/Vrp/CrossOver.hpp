/* 
 * File:   CrossOver.hpp
 * Author: francois
 *
 * Created on 24 novembre 2015, 11:42
 */

#ifndef CROSSOVER_HPP
#define	CROSSOVER_HPP

#include <genetic.hpp>

using namespace std;
using namespace genetic;

class CrossOver : public AbstractCrossOver {
public:
	CrossOver();
	virtual ~CrossOver();
	
	virtual int crossOver(Population::type &p1, Population::type &p2);
	
private:
	CrossOver(const CrossOver& orig);

};

#endif	/* CROSSOVER_HPP */

