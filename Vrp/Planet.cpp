/* 
 * File:   Town.cpp
 * Author: francois
 * 
 * Created on 19 novembre 2015, 14:53
 */

#include "Planet.hpp"
#include "GL/glew.h"
#include "Color.h"
#include <math.h>

#include <iostream>
#include "Widget.h"
#include "texture.hpp"
#include "Ring.hpp"
#include "FontRenderer.h"

using namespace std;

Planet::Planet(const string& name, const Point& p, float ray)
: mname(name), mpos(p), mray(ray), mcolor(Color::random()), mring(0)
{
}

void Planet::render(int quality) const {
	int lats = quality;
	int longs = lats;

	int i, j;

	glPushMatrix();
	mpos.renderGlTranslate();

	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0 = mray * sin(lat0);
		double zr0 = mray * cos(lat0);

		((*mcolor)*(0.5 + sin(lat0)*0.5)).render();
		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1 = mray * sin(lat1);
		double zr1 = mray * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}

	if (mring)
		mring->render(quality, mray);

	glPopMatrix();


}

void Planet::renderName() const {
	string n("helvetica 10");
	static FontRenderer* font = FontRenderer::factory(n);
	font->render(mpos.x(), mpos.y(), mname);

}

/*
void Planet::render() const {
	cout << '.';
	Color::yellow.render();
	drawSphere(mray, 10,10);
	return;
	glBegin(GL_LINES);
	glVertex3f(mpos.x() - 5, mpos.y() - 5, mpos.z());
	glVertex3f(mpos.x() - 5, mpos.y() - 5, mpos.z() + 10);
	glEnd();
	glBegin(GL_LINES);
	for (float x = -1; x < 1; x += 0.1) {
		Color::red.render();
		glVertex2f(x, 0.0);
		glVertex2f(x, 1.0);
	}
	glEnd();

	glBegin(GL_QUADS);
	Color::dark_gray.render();
	float s = 1;
	float y = 0;
	glVertex3f(-s, y, -s);
	glVertex3f(-s, y, s);
	glVertex3f(s, y, s);
	glVertex3f(s, y, -s);
	glEnd();
}
 */

float Planet::distanceTo(const Planet* t) const {
	return mpos.distance(t->pos());
}
