/* 
 * File:   Bati.hpp
 * Author: francois
 *
 * Created on 23 janvier 2017, 11:16
 */

#ifndef BATI_HPP
#define BATI_HPP

#include <MovingCoord.hpp>

namespace Colib
{
	class Colib;
	
	class Bati
	{
	public:
		Bati(const Colib*);
		void render();
		
	private:
		void pilier(int x, int y);
		
		static const int BATI_THICK = 10;
		float y;	
		int dest_y;
		float vy;	// actual speed 
		float ay;	// vertical acceleration
		
		const Colib* pcolib;
		
		MovingCoord h;
	};
}

#endif /* BATI_HPP */

