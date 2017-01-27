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
	class Navette;
	
	class Bati
	{
	public:
		Bati(const Colib*);
		
		/**
		 * 
		 * @return true if need to redraw soon
		 */
		bool render();
		
		float getHeight() const;	// Hauteur des traverses (cm)
		float getTopHeight() const;	// Hauteur des traverses + ep.
		int getLength() const;
		int getXLeft() const;
		int getXRight() const;
		
		void moveTo(int z_nocol, int h_percent);
		static const int BATI_THICK = 10;
		
	private:
		void pilier(int x, int z);
		void traverses();
		void traverse(int x);
		
		float y;	
		int dest_y;
		float vy;	// actual speed 
		float ay;	// vertical acceleration
		
		const Colib* pcolib;
		Navette* navette;
		
		MovingCoord h;	// 0..100% of height
	};
}

#endif /* BATI_HPP */

