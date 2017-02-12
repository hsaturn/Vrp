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
	class MotorSpeedHook;
	
	class Bati
	{
		static const int FACTOR=2;
		static const int MAX_SPEED=8*FACTOR;
	public:
		Bati(const Colib*);
		~Bati();
		
		/**
		 * 
		 * @return true if need to redraw soon
		 */
		bool render();
		
		// Is it ready to move somewhere ?
		bool isReady();
		
		// Is something moving somewhere ?
		bool isAllStopped();
		
		float getHeight() const;	// Hauteur des traverses (cm)
		float getTopHeight() const;	// Hauteur des traverses + ep.
		int getLength() const;
		int getXLeft() const;
		int getXRight() const;
		
		bool moveTo(int col_nr, int h, bool back);
		static const int THICKNESS = 10;
		
		Plateau* getPlateau();
		void setPlateau(Plateau* p);
		
		const char* get(bool back);	// récupère plateau, ret: erreur ou 0
		const char* put(bool back);	// pose le plateau, ret: erreur ou 0
		void remove();	// Enleve le plateau de la navette
		
		/** Commande de changement de son
		 * incoming = navette { sound generator }  : son de la navette, hook est le hook pour la vitesse
		 * incoming = bati {sound generator}       : son du bati (hook pour la vitesse)
		 */
		void changeSound(string &incoming);
		
		const Colib* pcolib;
		
	private:
		void pilier(int x, int z);
		void traverses();
		void traverse(int x);
		
		float y;	
		int dest_y;
		float vy;	// actual speed 
		float ay;	// vertical acceleration
		
		Navette* navette;
		int column_dest;
		int etage_dest;
		
		MovingCoord h;	// 0..100% of height
		
		MotorSpeedHook* hook_speed;
	};
}

#endif /* BATI_HPP */

