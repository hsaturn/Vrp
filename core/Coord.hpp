/* 
 * File:   Coord.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 21:24
 */

#pragma once

#include "Application.h"
#include <iostream>

/**
 Class used to store coordinates that can be used as key (for maps etc.) */

template<class Type>
class CoordType {
public:

	CoordType() {
		key = 0;
		_x = 0;
		_y = 0;
		_z = 0;
	};
	
	CoordType(Type cx, Type cy, Type cz) {
		_x = cx;
		_y = cy;
		_z = cz;
		key = _x + 1000.0 * _y + 1000000.0 * _z;
	};

	Type x() const {
		return _x;
	}

	Type y() const {
		return _y;
	}

	Type z() const {
		return _z;
	}

	inline bool operator<(const CoordType& b) const {
		return key < b.key;
	}

	inline void render() {
			glVertex3f(_x, _y, _z);
		};
		
	friend inline ostream& operator<<(ostream& out, const CoordType& c) {
		out << '(' << c.key << ", " << (int) c.x() << ", " << (int) c.y() << ", " << (int) c.z() << ", " << ')';
		return out;
	}

private:

	Type _x;
	Type _y;
	Type _z;
	float key;
};

typedef CoordType<float> Coord;
