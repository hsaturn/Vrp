/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "MovingCoord.hpp"
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std;

float sign(float f)
{
	if (f>0)
		return 1.0;
	else if (f<0)
		return -1.0;
	else
		return 0.0;
}

MovingCoord::MovingCoord(float v, float vmin, float vmax, float acc)
	:
	value(v),
	min(vmin),
	max(vmax),
	velocity(0),
	max_velocity(10),
	max_velocity_threshold(2),
	position_tolerance(0.1),
	accel(acc),
	target(v)
{
	last = std::chrono::system_clock::now();
}

void MovingCoord::setMaxVelocityThreshold(float m)
{
	max_velocity_threshold = abs(m);
}

void MovingCoord::setAccel(float a)
{
	accel = abs(a);
}

void MovingCoord::setMaxVelocity(float mv)
{
	max_velocity = abs(mv);
}

float MovingCoord::getDistance() const
{
	return abs((float)(target-value));
}

bool MovingCoord::targetReached() const
{
	return abs(target-value)<=position_tolerance;
}

void MovingCoord::setTarget(float t)
{
	if (t>max)
		t=max;
	else if (t<min)
		t=min;
	target=t;
}

void MovingCoord::update()
{
	// FIXME vmin, vmax
	std::chrono::time_point<std::chrono::system_clock> cur;
	
	cur = std::chrono::system_clock::now();
	
	std::chrono::duration<float> elapsed_seconds = cur-last;
	
	if (elapsed_seconds.count() == 0.0) return;
	
	last=cur;
	
//	if (target == value) return;
	
	float target_velocity;
	
	if (targetReached())
		target_velocity = 0;
	else
	{
		target_velocity = max_velocity * (target-value) / max_velocity_threshold;
		if (abs(target_velocity)>max_velocity)
			target_velocity = sign(target_velocity)*max_velocity;
	}
	
	float dv = accel * elapsed_seconds.count();
	
	if (velocity < target_velocity)
	{
		velocity += dv;
		if (velocity > target_velocity)
			velocity = target_velocity;
	}
	else
	{
		velocity -= dv;
		if (velocity < target_velocity)
			velocity = target_velocity;
	}
	
	value += velocity * elapsed_seconds.count();
}
	
