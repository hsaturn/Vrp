/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Axis.hpp
 * Author: hsaturn
 *
 * Created on 21 août 2017, 10:44
 */

#ifndef AXIS_HPP
#    define AXIS_HPP

class Axis
{
  public:
	Axis(float axis_length=1.0, bool axis_visible=false) : length(axis_length), active(axis_visible) {};
	virtual ~Axis();

	void render();
	void on() { active = true; }
	void off() { active = false; }
	void toggle() { active = !active; }
	bool isActive() const { return active; }
	
  private:
	
	float length;
	bool active;
};

#endif /* AXIS_HPP */

