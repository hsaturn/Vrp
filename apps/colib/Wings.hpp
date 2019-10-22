/* 
 * File:   Wings.hpp
 * Author: hsaturn
 *
 * Created on 16 février 2017, 19:56
 */

#pragma once

#include <core/DynamicFloat.hpp>

class Model;

namespace Colib
{
	class Plateau;
	class Bati;
	
	class Wings
	{
		public:
			Wings();
			bool render(float cx, float ytop, float center_z, const Plateau*, const Bati*);
			
			bool isAllStopped();
			
			void adjustFor(const Plateau*);
			void adjustFor(const Plateau*, bool back);
			
		private:
			DynamicFloat wings_z;	// Etirement des ailes (0 .. largeur plateau_z/2)
			DynamicFloat pusher_x;
			
			const Plateau* adjust;			// Ajustement sur ce plateau lors de render (attn this ptr must not be used to call Plateau methods))
			float pusher_offset_x;	// x du plateau d'ajustement

			static const Model* support_plateau_left;
			static float offset_support_left[3];
			
			static const Model* support_plateau_right;
			static float offset_support_right[3];
			
			static const Model* pusher;
			static const Model* pusher_support;		// Model must be centered on x & z
			static float pusher_support_offset_y;

		private:
			void loadModels();
			void checkCentered(const Model*, const char* name);
	};
}
