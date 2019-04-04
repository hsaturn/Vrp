/* 
 * File:   Background.cpp
 * Author: hsaturn
 * 
 * Created on 19 novembre 2015, 23:53
 */

#include "Background.h"
#include <iostream>
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

	st_star(float x, float y, float z) : mx(x), my(y), mz(z) {
	};
	
	st_star() {
		//Random::rand(-10, 10), Random::rand(-10, 10),  Random::rand(-10, 10)));
		
		float ray=100.0;
		float angle1 = Random::rand(0, M_PI);
		float angle2 = Random::rand(0, 2*M_PI);
				
		mx = sin(angle1)*cos(angle2)*ray;
		my = sin(angle1)*sin(angle2)*ray;
		mz = cos(angle1)*ray;
	}
	
	void render()
	{
		glVertex3f(mx, my, mz);
	}
	
private:
	
	float mx;
	float my;
	float mz;
	
};

void BackgroundStars::render() {
	static map <float, vector < st_star *>> stars;
	const int FACT=10;
	if (stars.size() == 0) {
		for (float size = 0.5; size < 2; size+= 0.5) {
			int count = Random::rand(5*FACT, 15*FACT);
			for (int i = 0; i < count; i++)
				stars[size].push_back(new st_star());
		}
		cout << "STARS : " << stars.size() << endl;
	}
	glEnable(GL_POINT_SMOOTH); 
	Color::white.render(0.7);
	for (auto it : stars) {
		glPointSize(it.first);
		glBegin(GL_POINTS);
		for (auto st : it.second)
			st->render();
		glEnd();
	}
}
