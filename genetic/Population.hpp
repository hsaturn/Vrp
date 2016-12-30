/* 
 * File:   Population.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:45
 */

#ifndef POPULATION_HPP
#define	POPULATION_HPP

#include <vector>
#include "Vector.hpp"
#include "AbstractIndividual.hpp"
#include <memory>

using namespace std;

namespace genetic {

	class Population : public vector<shared_ptr<AbstractIndividual>> {
	public:
		
		typedef shared_ptr<AbstractIndividual>	type;
		Population();
		virtual ~Population();
		
		Population::type chooseRandom() const;
		
	private:
		Population(const Population& orig);

	};

}

#endif	/* POPULATION_HPP */

