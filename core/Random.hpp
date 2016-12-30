/* 
 * File:   Random.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 15:08
 */

#ifndef RANDOM_HPP
#define	RANDOM_HPP
#include <stdint.h>

class Random {
	static uint64_t rand_next;
public:
	static void seed(uint64_t seed);
	static uint64_t rand();

	/* return 0..1 float */
	static float rand1();
	
	static float rand(float min, float max);
	
private:
	Random();
	Random(const Random& orig);
	virtual ~Random();

};

#endif	/* RANDOM_HPP */

