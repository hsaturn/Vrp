/* 
 * File:   Population.cpp
 * Author: francois
 * 
 * Created on 23 novembre 2015, 16:45
 */

#include "Population.hpp"
#include <stdlib.h>

namespace genetic
{

  Population::Population() { }

  Population::type Population::chooseRandom() const
  {
	  //assert(size()>0);
	  unsigned int ind = rand() % size();
	  return (*this)[ind];
  }

  Population::~Population()
  {
  }
}


