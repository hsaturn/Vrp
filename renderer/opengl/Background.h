/* 
 * File:   Background.h
 * Author: hsaturn
 *
 * Created on 19 novembre 2015, 23:53
 */
#pragma once
#include <math.h>

class AbstractBackground {
public:
	AbstractBackground(){};
	AbstractBackground(const AbstractBackground& orig);
	virtual ~AbstractBackground(){};
	
	virtual void render()=0;
private:

};

class BackgroundStars : public AbstractBackground
{
public:
	BackgroundStars() : AbstractBackground(){}
	
	virtual ~BackgroundStars(){};
	
	virtual void render();
};

