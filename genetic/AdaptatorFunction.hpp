/* 
 * File:   AdaptatorFunction.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:45
 */

#ifndef ADAPTATORFUNCTION_HPP
#define	ADAPTATORFUNCTION_HPP

// This abstract class is use to implement the adapation functor that will
// score each population individual

#include "AbstractIndividual.hpp"
#include "Population.hpp"
#include <memory>

using namespace std;

namespace genetic {

	class AdaptatorFunction {
	public:
		AdaptatorFunction(){};
		virtual ~AdaptatorFunction() {};

		// Higher the score is, better the individual is.
		virtual float fitnessScore(Population::type) const=0;

	private:
		AdaptatorFunction(const AdaptatorFunction& orig);

	};

}
#endif	/* ADAPTATORFUNCTION_HPP */

