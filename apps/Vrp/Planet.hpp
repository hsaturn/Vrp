/* 
 * File:   Town.hpp
 * Author: francois
 *
 * Created on 19 novembre 2015, 14:53
 */

#ifndef TOWN_HPP
#define	TOWN_HPP

#include <string>
#include <Point.hpp>


using namespace std;

class AbstractRing;
class Color;

class Planet {
public:
	Planet(const string& name, const Point& p, float ray);
	
	void render(int quality) const;
	void renderName() const;

	float ray() const { return mray; }
	float distanceTo(const Planet*) const;
	
	bool collide(const Planet* p, float securityDistance=0) const { return distanceTo(p)>mray+p->mray+securityDistance; } 
	
	const Point& pos() const { return mpos; }
	const Color* color() const { return mcolor; }
	
	void setRing(const AbstractRing* r) { mring=r; }
	const string& getName() const { return mname; }
	
private:
	string mname;
	Point mpos;
	float mray;
	const Color* mcolor;
	const AbstractRing* mring;
};

#endif	/* TOWN_HPP */

