/* 
 * File:   AbstractIndividual.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:48
 */

#ifndef ABSTRACTINDIVIDUAL_HPP
#define	ABSTRACTINDIVIDUAL_HPP

#include <unordered_map>
#include <ostream>

using namespace std;
// An individual is a collection of gene.

namespace genetic {

	class AbstractIndividual {
	public:
		AbstractIndividual();
		
		virtual ~AbstractIndividual();
		
		virtual void mutate();	// TODO generic mutation needs to move Indivual.cpp container code here
		virtual void render(int quality);
		virtual void dump(ostream&) const {};

	private:
		AbstractIndividual(const AbstractIndividual& orig);

	};

}
#endif	/* ABSTRACTINDIVIDUAL_HPP */

