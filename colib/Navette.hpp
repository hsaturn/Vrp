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

namespace Colib
{
	class Bati;
	
	class Navette
	{
		public:
			Navette(Bati* bati);
		
			/**
			 * @return  true if need to redraw soon
			 */
			bool render();
			
			void moveTo(int z_percent);
		
		private:
			static const int NAV_LENGTH = 40;	// width (cm)
			static const int NAV_HEIGHT = 30;	// height (cm)
			
			Bati* pbati;
			
			MovingCoord z;	// 0..100% of length

	};
}

#endif /* NAVETTE_HPP */

