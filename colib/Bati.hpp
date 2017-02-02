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
	class Plateau;
	
	class Bati
	{
	public:
		Bati(const Colib*);
		
		/**
		 * 
		 * @return true if need to redraw soon
		 */
		bool render();
		bool isReady();
		
		float getHeight() const;	// Hauteur des traverses (cm)
		float getTopHeight() const;	// Hauteur des traverses + ep.
		int getLength() const;
		int getXLeft() const;
		int getXRight() const;
		
		bool moveTo(int z, int h);
		static const int THICKNESS = 10;
		
		Plateau* getPlateau();
		void setPlateau(Plateau* p);
		
		
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

