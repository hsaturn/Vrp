/* 
 * File:   Point.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 15:02
 */

#pragma once
#include "glm/glm.hpp"

class Point {
public:
	Point(float x, float y, float z=0) : mx(x),my(y),mz(z){}

	float distance(const Point&) const;
	
	float x() const { return mx; }
	float y() const { return my; }
	float z() const { return mz; }
	
	void render() const;
	void renderGlTranslate() const;
	void translate(glm::vec3);
	
private:
	float mx;
	float my;
	float mz;

};

