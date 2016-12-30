/* 
 * File:   Range.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 15:05
 */

#ifndef RANGE_HPP
#define	RANGE_HPP

#include <Point.hpp>

class Range {
public:
	Range(float x, float y, float z) : mx(x), my(y), mz(z) {};
	
	Point createRandomPoint(int x=1, int y=1, int z=1) const;

	float x() const { return mx; }
	float y() const { return my; }
	float z() const { return mz; }
private:
	float mx;
	float my;
	float mz;
};

#endif	/* RANGE_HPP */

