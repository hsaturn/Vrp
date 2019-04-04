/* 
 * File:   Point.cpp
 * Author: francois
 * 
 * Created on 19 novembre 2015, 15:02
 */

#include "Point.hpp"
#include <math.h>
#include <GL/glew.h>

void Point::render() const
{
	glVertex3f(mx,my,mz);
}

void Point::renderGlTranslate() const {
	glTranslatef(mx,my,mz);
}

void Point::translate(glm::vec3 v)
{
	mx += v.x;
	my += v.y;
	mz += v.z;
}
float Point::distance(const Point& p) const
{
	return sqrt(pow(p.mx-mx,2)+pow(p.my-my,2)+pow(p.mz-mz,2));
}
