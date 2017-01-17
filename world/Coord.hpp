/* 
 * File:   Coord.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 21:24
 */

#ifndef COORD_HPP
#define	COORD_HPP

#include "Object.h"
#include <iostream>


namespace World
{
	typedef float Type;

	/**
	 Class used to store coordinates that can be used as key (for maps etc.) */
	
class Coord {
public:
	Coord();
	Coord(Type x, Type y, Type z);
	
	Type x() const { return _x; }
	Type y() const { return _y; }
	Type z() const { return _z; }
	
	bool operator < (const Coord& b) const
	{
		return key < b.key;
	}

	
	friend inline ostream& operator << (ostream& out, const Coord& c)
	{
		out << '(' << c.key << ", " << (int)c.x() << ", " << (int)c.y() << ", " << (int)c.z() << ", "<<  ')';
		return out;
	}
	
	void render();
	
private:
	
	Type _x;
	Type _y;
	Type _z;
	float key;
};

}
#endif	/* COORD_HPP */

