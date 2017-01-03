/* 
 * File:   Torch.hpp
 * Author: francois
 *
 * Created on 6 avril 2016, 15:37
 */

#ifndef TORCH_HPP
#define	TORCH_HPP

#include "Block.hpp"

namespace World {

	class Torch : public Block {
		
	public:
		Torch(string &incoming);
		virtual ~Torch(){};
		
		virtual void render(Coord c, const World*);
		
	private:
		Torch(const Torch& orig);

		Side side;
	};

}

#endif	/* TORCH_HPP */

