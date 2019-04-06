/* 
 * File:   CrossOver.hpp
 * Author: francois
 *
 * Created on 24 novembre 2015, 11:42
 */

#pragma once

#include <genetic.hpp>

using namespace std;
using namespace genetic;

class CrossOver : public AbstractCrossOver {
public:
	CrossOver();
	~CrossOver() override;
	
	int crossOver(Population::type &p1, Population::type &p2) override;
	
private:
	CrossOver(const CrossOver& orig);

};
