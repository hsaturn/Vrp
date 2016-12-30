/* 
 * File:   AbstractIndividualFactory.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:59
 */

#ifndef ABSTRACTINDIVIDUALFACTORY_HPP
#define	ABSTRACTINDIVIDUALFACTORY_HPP

#include "Population.hpp"

namespace genetic {

	class AbstractIndividual;

	class AbstractIndividualFactory {
	public:
		AbstractIndividualFactory();
		virtual ~AbstractIndividualFactory();

		virtual Population::type build() = 0;

	private:
		AbstractIndividualFactory(const AbstractIndividualFactory& orig);
	};

}
#endif	/* ABSTRACTINDIVIDUALFACTORY_HPP */

