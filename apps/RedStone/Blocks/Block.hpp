/* 
 * File:   Bloc.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:35
 */

#pragma once

#include <Coord.hpp>

namespace RedStone {
	class RedMap;

	// A block is rendered at coord c
	// from c.x, c.y, c.z to c.x+1, c.y+1, c.z+1

	class Block {
	public:
		Block();

		virtual ~Block() {
		};

		// z+ aller vers le sud
		// x+ aller vers l'est

		enum Side {
			SOUTH,
			WEST,
			NORTH,
			EAST,
			VERTICAL
		};



		// Draw a simple cube
		void cubeRender(const Coord &c, bool borders = false) const;

		virtual void render(Coord c, const RedMap*) = 0;

	private:

	};
}

