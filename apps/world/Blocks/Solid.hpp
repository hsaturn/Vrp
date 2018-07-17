/* 
 * File:   Solid.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 21:12
 */

#ifndef SOLID_HPP
#define	SOLID_HPP

#include "Block.hpp"
#include <string>

using namespace std;

namespace World
{
	
class Solid : public Block {
public:
	Solid(string& incoming);
	~Solid() override {}
	
	void render(Coord c, const World*) override;

private:
	Solid(const Solid& orig);

};

}
#endif	/* SOLID_HPP */

