/* 
 * File:   Ring.hpp
 * Author: hsaturn
 *
 * Created on 19 novembre 2015, 22:32
 */

#ifndef RING_HPP
#define	RING_HPP

class AbstractRing {
public:
	AbstractRing(){};
	virtual ~AbstractRing(){};
	virtual void render(int quality, float ray) const=0;
	
private:
	AbstractRing(const AbstractRing& orig);
};

class RainbowRing : public AbstractRing
{
public:
	RainbowRing() : AbstractRing() {}
	virtual ~RainbowRing() {};
	virtual void render(int quality, float ray) const;
};
#endif	/* RING_HPP */

