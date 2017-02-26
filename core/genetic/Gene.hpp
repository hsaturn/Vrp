/* 
 * File:   Gene.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:49
 */

#ifndef GENE_HPP
#define	GENE_HPP

namespace genetic {

	class Gene {
	public:
		Gene();
		virtual ~Gene();

	private:
		Gene(const Gene& orig);

	};

}
#endif	/* GENE_HPP */

