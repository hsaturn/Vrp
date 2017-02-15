/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Plateau.hpp
 * Author: francois
 *
 * Created on 2 février 2017, 11:58
 */

#ifndef PLATEAU_HPP
#define PLATEAU_HPP

#include "MovingCoord.hpp"
#include <string>

using namespace std;

class Model;

namespace Colib {
	class MotorSpeedHook;

	class Plateau
	{
	public:
		static const int THICKNESS = 2;	// Height
		
		bool isReady() const { return x.targetReached(); }
		bool isAllStopped() const { return x.targetReached(); }
		void setTargetX(int tx) { x.setTarget(tx); }
		const string& getContent() { return mcontent; }
		float getX() const { return x; }
		int getWidth() const { return width_z; }
		int getHeight() const { return height_y; }
		
		const Model* getModel() const { return pmodel; }

		const MovingCoord& getMovingCoord() const { return x; }
		/**
		 * @param cx center coords
		 * @param cy
		 * @param cz
		 */
		void renderAtCenter(float cy, float cz);
		
		static Plateau* factory(string content, int center_x);
		
		~Plateau();
		
	private:
		Plateau(string content, int center_x);
		MovingCoord x;
		string unique_name;
		string mcontent;
		const Model* pmodel;
		int length_x;
		int height_y;
		int width_z;
		MotorSpeedHook* speed_hook;
	};
}


#endif /* PLATEAU_HPP */

