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

namespace Colib
{
	class Colib;
	
	class Navette
	{
		public:
			Navette(Colib* colib) : m_colib(colib){}
		
			void _render(bool resetTimer);
		
		private:
			Colib* m_colib;

	};
}

#endif /* NAVETTE_HPP */

