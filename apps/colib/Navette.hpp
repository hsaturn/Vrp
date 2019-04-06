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

#pragma once

#include <Object.h>
#include <DynamicFloat.hpp>
#include <model/Model.hpp>

using namespace std;

namespace Colib
{
	class Column;
	class Bati;
	class Plateau;
	class MotorSpeedHook;
	class Wings;
	
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
			
			const char* canGet(Column* col, int etage);			// ret: 0 if can or reason if cannot
			const char* get(Column* col, int etage, bool back);			// ret: err ou 0
			void remove();
			
			void centerOn(float z, bool back);
			static const int LENGTH_Z = 40;	// SIZE Z (cm)
			static const int HEIGHT_Y = 10;	// SIZE Y (cm)
			
			void changeSound(string& incoming);
			
		private:
			
			Bati* pbati;
			
			DynamicFloat z;
			Plateau* plateau;
			
			Column* moving_col;	// 0 ou ptr column put/get plateau (en cours)
			bool putting;		// si moving_col, putting / getting plateau
			bool putting_back;	// si moving_col && putting, de quel coté ?
			int etage_dest;		// etage dest / source
			MotorSpeedHook* speed_hook;	// Pour le générateur de son
			Wings* wings;
	};
}
