/* 
 * File:   Ring.cpp
 * Author: hsaturn
 * 
 * Created on 19 novembre 2015, 22:32
 */

#include "Ring.hpp"
#include "Planet.hpp"
#include <math.h>
#include <Color.h>
#include <map>

using namespace std;

void RainbowRing::render(int quality, float ray) const {
	ray *=1.3;
	int q = quality;
	float d = (M_PI * 2) / q;
	float x, z;
	const Color& c = Color::dark_gray;
	float coeff2 = 0.5;
	map<float, float> gen ={
		{ 0.2, 0.3}, // ray, alpha de 0 à 0.2
		{ 0.3, 0.2},
		{ 0.4, 0 },
		{ 0.7, 0.5},
		{ 0.8, 0.7},
		{ 1.0, 0.0},
		{ 1.2, 0.7},
		{ 1.3, 0.3},
		{ 1.4, 0.5}
		
	};
	
	float coeffalpha = 0.5;

	float a2 = 0;
	float a1;
	for (auto it : gen) {
		a1 = a2;
		a2 = it.first;
		c.render(it.second*coeffalpha);

		glBegin(GL_QUAD_STRIP);
		float k = 0;
		for (int i = 0; i <= q; i++) {
			x = sin(k) * (ray + a1 * coeff2);
			z = cos(k) * (ray + a1 * coeff2);
			glVertex3f(x, z, x);
			x = sin(k) * (ray + a2 * coeff2);
			z = cos(k) * (ray + a2 * coeff2);
			glVertex3f(x, z, x);
			k += d;
		}
		glEnd();
	}
}

/* disque avec dégradé opacité
void RainbowRing::render(float ray) const {
	float d = (M_PI * 2 )/ Planet::getQuality();
	float x,z;
	const Color& c=Color::dark_gray;
	float coeff=0.5;
	float coeff2=0.5;
	for (float alpha_gen = -0.9; alpha_gen < 0.9; alpha_gen += 0.2) {
		if (alpha_gen<0)
			c.render(coeff*(1+alpha_gen));
		else
			c.render(coeff*(1-alpha_gen));
	glBegin(GL_LINE_LOOP);
		for (float k = 0; k < (M_PI * 2); k += d) {
			x = sin(k) * (ray+alpha_gen*coeff2);
			z = cos(k) * (ray+alpha_gen*coeff2);
			glVertex3f(x, z, x);
		}
	glEnd();
	}
}*/
