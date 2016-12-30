/* 
 * File:   Background.cpp
 * Author: hsaturn
 * 
 * Created on 19 novembre 2015, 23:53
 */

#include "Background.h"
#include <iostream>
#include <GL/glew.h>
#include "Random.hpp"
#include "Color.h"
#include <map>
#include <vector>

using namespace std;

void drawStar() {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();
}

class st_star {
public:

	st_star(float x, float y) : mx(x), my(y) {
	};
	float mx;
	float my;
};

void BackgroundStars::render() {
	static map <float, vector < st_star *>> stars;
	if (stars.size() == 0) {
		for (float size = 0.5; size < 2; size+= 0.5) {
			int count = Random::rand(20, 100);
			for (int i = 0; i < count; i++)
				stars[size].push_back(new st_star(Random::rand(-10, 10), Random::rand(-10, 10)));
		}
		cout << "STARS : " << stars.size() << endl;
	}
	glEnable(GL_POINT_SMOOTH); 
	Color::white.render(0.7);
	for (auto it : stars) {
		glPointSize(it.first);
		glBegin(GL_POINTS);
		for (auto st : it.second) {
			glVertex2f(st->mx, st->my);
		}
		glEnd();
	}
}