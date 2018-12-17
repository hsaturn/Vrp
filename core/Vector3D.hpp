/* 
 * File:   Vector3D.hpp
 * Author: francois
 *
 * Created on 6 avril 2016, 17:27
 */

#ifndef VECTOR3D_HPP
#define	VECTOR3D_HPP

#include <math.h>

#define sqr(a) (a*a)

class Vector3D {
public:

	Vector3D() : x(0), y(0), z(0) {
	}

	Vector3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {
	}

	void roundMine() {
		x = floor(x) + 0.5;
		y = round(y);
		z = floor(z) + 0.5;
	}

	bool equals(Vector3D v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	/* vector3d.prototype.out = function (txt) {
	if (typeof (txt) == "undefined")
		txt = "";
	else
		txt += " ";
	clientMessage(txt + round2(this.x) + ' ' + round2(this.y) + " " + round2(this.z) + " len=" + round2(this.length()));
};	*/


	void mul(float k) {
		x *= k;
		y *= k;
		z *= k;
	}

	void add(const Vector3D& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void substract(const Vector3D&v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void normalize() {
		float l = length();
		if (l == 0)
			return;
		x /= l;
		y /= l;
		z /= l;
	}

	float distanceTo(const Vector3D&v) {
		return sqrt(
				sqr(v.x - x) +
				sqr(v.y - y) +
				sqr(v.z - z));
	}

	void barycenterWith(const Vector3D&v) {
		x = (x + v.x) / 2;
		y = (y + v.y) / 2;
		z = (z + v.z) / 2;
	}

	float length() {
		return sqrt(sqr(x) + sqr(y) + sqr(z));
	}

	Vector3D* clone() {
		return new Vector3D(x, y, z);
	}

	Vector3D* cross(const Vector3D& v) { // FIXME MEMORY LEAK
		return new Vector3D(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
				);
	}

	float scalar(const Vector3D& v) {
		return x * v.x + y + v.y + z * v.z;
	}

	void render() {
		glVertex3f(x, y, z);
	}


private:
	float x, y, z;


};

#endif	/* VECTOR3D_HPP */

