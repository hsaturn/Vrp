/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MotorSpeedHook.hpp
 * Author: hsaturn
 *
 * Created on 10 février 2017, 02:14
 */
#ifndef MOTORSPEEDHOOK_HPP
#define MOTORSPEEDHOOK_HPP

#include <atomic>
#include <string>

#ifdef HAVE_SYNTH
#include <libsynth.hpp> 
#endif

using namespace std;

namespace Colib
{
#ifdef HAVE_SYNTH
	class MotorSpeedHook
	: public SoundGeneratorVarHook<float>
#else
	class MotorSpeedHook
#endif
	{
		public:
			MotorSpeedHook(string name, float max_speed);
			~MotorSpeedHook();

			void update(float f);

			void changeSound(string& in);
			void deleteSound();

		private:
			atomic<float> speed;
			string hook_name;
	#ifdef HAVE_SYNTH
			SoundGenerator* sound;
	#endif
	};
}

#endif /* MOTORSPEEDHOOK_HPP */