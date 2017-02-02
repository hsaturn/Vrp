/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Navette.hpp
 * Author: francois
 *
 * Created on 23 janvier 2017, 10:28
 */

#ifndef NAVETTE_HPP
#define NAVETTE_HPP

#include <Object.h>
#include <MovingCoord.hpp>

using namespace std;

namespace Colib
{
	class Bati;
	class Plateau;
	
	class Navette
	{
		public:
			Navette(Bati* bati);
		
			/**
			 * @return  true if need to redraw soon
			 */
			bool render();
			
			bool isReady() const;
			
			Plateau* getPlateau() { return plateau; }
			void setPlateau(Plateau* p);
			
			void centerOn(int z);
			static const int LENGTH = 40;	// width (cm)
			static const int HEIGHT = 10;	// height (cm)
		
		private:
			
			Bati* pbati;
			
			MovingCoord z;	// 0..100% of length
			Plateau* plateau;
	};
}

#endif /* NAVETTE_HPP */

