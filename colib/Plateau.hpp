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

namespace Colib {
	class Plateau
	{
	public:
		Plateau(string content) : mcontent(content){};
		static const int LENGTH = 60;	// Z axis
		static const int HEIGHT = 2;	// Height
		
		bool isReady() const { return x.targetReached(); }
		void setTargetX(int tx) { x.setTarget(x); }
		const string& getContent() { return mcontent; }

		/**
		 * @param cx center coords
		 * @param cy
		 * @param cz
		 */
		void renderAtCenter(int cy, int cz);
		
	private:
		MovingCoord x;
		string mcontent;
	};
}


#endif /* PLATEAU_HPP */

