/* 
 * File:   Range.cpp
 * Author: francois
 * 
 * Created on 19 novembre 2015, 15:05
 */

#include "Range.hpp"
#include "Random.hpp"

Point Range::createRandomPoint(int x, int y, int z) const
{
	return Point(
				x ? (Random::rand1() * mx - Random::rand1() * mx)/2 : 0,
				y ? (Random::rand1() * my - Random::rand1() * my)/2 : 0,
				z ? (Random::rand1() * mz - Random::rand1() * mz)/2 : 0);
}
