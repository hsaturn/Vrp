/* 
 * File:   Bati.hpp
 * Author: francois
 *
 * Created on 23 janvier 2017, 11:16
 */
#pragma once

#include <Object.h>
#include <DynamicFloat.hpp>
#include "application/IRenderable.hpp"

namespace Colib
{
	class ColibApp;
	class Navette;
	class Plateau;
	class MotorSpeedHook;
	
	class Bati: public IRenderable
	{
		static const int FACTOR=2;
		static const int MAX_SPEED=8*FACTOR;
		
	public:
		Bati(const ColibApp*);
		~Bati();
		
		/**
		 * 
		 * @return true if need to redraw soon
		 */
		bool _render(bool) override;
		
		// Is it ready to move somewhere ?
		bool isReady();
		
		// Is something moving somewhere ?
		bool isAllStopped();
		
		float getHeight   () const;	// Hauteur des traverses (cm)
		float getTopHeight() const;	// Hauteur des traverses + ep.
		float getLength   () const;
		float getXLeft    () const;
		float getXRight   () const;
		float getCenterX  () const;
		
		IRunnable::ExecResult moveTo(int col_nr, int h, bool back);
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
		void changeSound(string incoming);
		
		const ColibApp* pcolib;
		
	private:
		void pilier(int x, int z);
		void traverses(float delta);
		void traverse(int x, float delta);
		
		float y;	
		int dest_y;
		float vy;	// actual speed 
		float ay;	// vertical acceleration
		
		Navette* navette;
		int column_dest;
		int etage_dest;
		
		DynamicFloat h;	// 0..100% of height
		
		MotorSpeedHook* hook_speed;
	};
}
