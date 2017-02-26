/* 
 * File:   AbstractCrossOver.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 18:09
 */

#ifndef ABSTRACTCROSSOVER_HPP
#define	ABSTRACTCROSSOVER_HPP

#include "Population.hpp"

namespace genetic {

	class Population;
	class AbstractIndividual;

	class AbstractCrossOver {
	public:

		/**
		 * Combine two individuals each others and return the (new) individuals in p1 & p2
         * @param p1
         * @param p2
		 * @return int 0 (no cross computed, 1 (p1 computed), 2 (p1 & p2 are computed)
         */
		virtual int crossOver(Population::type &p1, Population::type &p2)=0;

		AbstractCrossOver();
		virtual ~AbstractCrossOver();

	private:
		AbstractCrossOver(const AbstractCrossOver& orig);

	};

}
#endif	/* ABSTRACTCROSSOVER_HPP */

