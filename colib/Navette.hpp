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
	class Column;
	class Bati;
	class Plateau;
	class MotorSpeedHook;
	
	class Navette
	{
		public:
			Navette(Bati* bati);
			~Navette();
		
			static const int FACTOR=4;
			static const int MAX_SPEED = FACTOR*5;

			/**
			 * @return  true if need to redraw soon
			 */
			bool render();
			
			// Ready to move
			bool isReady() const;
			
			// Nothing is moving around this
			bool isAllStopped() const;
			
			Plateau* getPlateau() { return plateau; }
			void setPlateau(Plateau* p);
			
			const char* put(Column* col, int etage, bool back);	// ret: err ou 0
			const char* get(Column* col, int etage);			// ret: err ou 0
			void remove();
			
			void centerOn(float z);
			static const int LENGTH_Z = 40;	// SIZE Z (cm)
			static const int HEIGHT_Y = 10;	// SIZE Y (cm)
			
			void changeSound(string& incoming);
			
		private:
			
			Bati* pbati;
			
			MovingCoord z;
			Plateau* plateau;
			
			Column* moving_col;	// 0 ou ptr column put/get plateau (en cours)
			bool putting;		// si moving_col, putting / moving plateau
			int etage_dest;		// etage dest / source
			MotorSpeedHook* speed_hook;	// Pour le générateur de son
	};
}

#endif /* NAVETTE_HPP */

