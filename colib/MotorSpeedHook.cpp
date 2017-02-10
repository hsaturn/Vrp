/* 
 * File:   MotorSpeedHook.cpp
 * Author: hsaturn
 * 
 * Created on 10 février 2017, 02:14
 */

#include "MotorSpeedHook.hpp"
#include "Navette.hpp"
#include <cmath>

namespace Colib
{
	void MotorSpeedHook::changeSound(istream& in)
	{
		speed = 0;
#ifdef HAVE_SYNTH
		deleteSound();
		sound = SoundGenerator::factory(in);
		SoundGenerator::play(sound);
#endif
	}
	
	MotorSpeedHook::MotorSpeedHook(string name, float speed_max)
#ifdef HAVE_SYNTH
		: SoundGeneratorVarHook(&speed, 0, speed_max, name)
	{
		sound = 0;
	}
#else
	{
		
	}
#endif


	void MotorSpeedHook::update(float f)
	{
		speed = (float)abs(f);
		return;
		if (f<0.02)
		{
			SoundGenerator::remove(sound);
		}
		else
			SoundGenerator::play(sound);
	}
	
	void MotorSpeedHook::deleteSound()
	{
		cout << "deleteSound() " << sound << endl;
#if HAVE_SYNTH
		if (sound)
		{
			cout << "DELETING SOUND" << SoundGenerator::remove(sound) << endl;
			delete sound;
			sound = 0;
		}
#endif
	}

	MotorSpeedHook::~MotorSpeedHook()
	{
		deleteSound();
	}
}