/* 
 * File:   MotorSpeedHook.cpp
 * Author: hsaturn
 * 
 * Created on 10 février 2017, 02:14
 */

#include "MotorSpeedHook.hpp"
#include "Navette.hpp"
#include <cmath>
#include <StringUtil.hpp>

namespace Colib
{
	void MotorSpeedHook::changeSound(string& sound_str)
	{
		speed = 0;
#ifdef HAVE_SYNTH
		stringstream in;
		while(sound_str.length())
		{
			string s = StringUtil::getWord(sound_str);
			if (s=="hook") s=hook_name;
			in << s << ' ';
		}
		deleteSound();
		cout << "Changing sound  : " << in.str() << endl;
		sound = SoundGenerator::factory(in);
		cout << "Play sound " << sound << endl;
		SoundGenerator::play(sound);
#endif
	}
	
	MotorSpeedHook::MotorSpeedHook(string name, float speed_max)
#ifdef HAVE_SYNTH
		: SoundGeneratorVarHook(&speed, 0, speed_max, name)
	{
		hook_name = name;
		sound = 0;
	}
#else
	{
		
	}
#endif


	void MotorSpeedHook::update(float f)
	{
#ifdef HAVE_SYNTH		
		f = abs(f);
		if (f<0.02) f=0;
		speed = (float)abs(f);
		return;
		if (f<0.02)
			SoundGenerator::remove(sound);
		else
			SoundGenerator::play(sound);
#endif
	}
	
	void MotorSpeedHook::deleteSound()
	{
#if HAVE_SYNTH
		cout << "deleteSound() " << sound << endl;
		if (sound)
		{
			cout << "DELETING SOUND " << SoundGenerator::remove(sound) << endl;
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