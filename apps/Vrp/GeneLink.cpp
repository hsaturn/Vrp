/* 
 * File:   GeneLink.cpp
 * Author: hsaturn
 * 
 * Created on 23 novembre 2015, 19:46
 */


#include "GeneLink.hpp"
#include "Planet.hpp"
#include <Color.h>

GeneLink::GeneLink() {
}

GeneLink::GeneLink(const GeneLink& orig) {
}

GeneLink::~GeneLink() {
}

void GeneLink::render() {
	Color::dark_gray.render(0.5);
	glBegin(GL_LINE);
	planetStart->pos().render();
	planetEnd->pos().render();
	glEnd();
}
