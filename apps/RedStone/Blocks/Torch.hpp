/* 
 * File:   Torch.hpp
 * Author: francois
 *
 * Created on 6 avril 2016, 15:37
 */

#pragma once

#include "Block.hpp"

namespace RedStone {

	class Torch : public Block {
		
	public:
		Torch(string &incoming);
		~Torch() override {}
		
		void render(Coord c, const RedMap*) override;
		
	private:
		Torch(const Torch& orig);

		Side side;
	};

}
