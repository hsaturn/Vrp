/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DynamicFloat.hpp
 * Author: francois
 *
 * Float value that can go from a start point to a end point with
 * a given acceleration / deceleration
 * 
 * Created on 23 janvier 2017, 11:22
 */

#ifndef DYNAMICFLOAT_HPP
#define DYNAMICFLOAT_HPP
#include <chrono>
#include <ostream>

using namespace std;

class DynamicFloat
{
public:
	DynamicFloat(float f=0, float min=0, float max=10, float accel=1);

	operator float() const { return value; }
	void setMinValue(float m) { min = m; }
	void setMaxValue(float m) { max = m; }
	
	/**
	 * set max speed in unit per second
	 * @param max_speed
	 */
	
	void update();
	
	void setMaxVelocityThreshold(float);
	void setMaxVelocity(float);
	
	// max dv/dt
	void setAccel(float);
	
	void setTarget(float t);
	float getTarget() const { return target; }
	float getDistance() const;
	bool targetReached() const;
	
	float getVelocity() const { return velocity; }
	
	void setPositionTolerance(float t) { position_tolerance = t; }
	
	float operator = (float f) { setValue(f); return f; }
	
	void setValue(float t);
	
	friend ostream& operator<<(ostream& out, const DynamicFloat&);
	
private:
	
	float value;
	float min;
	float max;
	float velocity;
	float max_velocity;
	float max_velocity_threshold;
	float position_tolerance;
	float accel;
	float target;
	
	std::chrono::time_point<std::chrono::system_clock> last;
};

#endif /* DYNAMICFLOAT_HPP */

