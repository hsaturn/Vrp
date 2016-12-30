/* 
 * File:   Random.cpp
 * Author: francois
 * 
 * Created on 19 novembre 2015, 15:08
 */

#include "Random.hpp"
uint64_t Random::rand_next=1;

uint64_t Random::rand(void)
{
	rand_next=rand_next*1103515245+12345;
	return rand_next;
}

float Random::rand1()
{
	return (float)(rand()%1000)/1000.0;
}

float Random::rand(float min, float max)
{
	return min+((float)(rand()%10000)/10000.0)*(max-min);
}

void Random::seed(uint64_t seed)
{
	rand_next = seed;
}
