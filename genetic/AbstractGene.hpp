/* 
 * File:   AbstractGene.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:49
 */

#ifndef ABSTRACTGENE_HPP
#define	ABSTRACTGENE_HPP

namespace genetic {

	class AbstractGene {
	public:
		AbstractGene(){};
		virtual ~AbstractGene(){};
		
		virtual void render(){};

	private:
		AbstractGene(const AbstractGene& orig);

	};

}
#endif	/* ABSTRACTGENE_HPP */

