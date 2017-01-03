/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: francois
 *
 * Created on 3 janvier 2017, 13:20
 */

#ifndef MAP_H
#define MAP_H

#include <stdint.h>

namespace World {
	
/*
 * A map is an 2 dimensions array of heights.
 * Each value is the height of a corner of a tile.
 */
class Map {
public:
	Map(int w, int l);
	~Map();
	void render();		// Open Gl Rendering !
	
private:
	Map(const Map& orig);

private:
	int width;
	int length;
	
	uint8_t* heights;	// height[w][h]
};

}

#endif /* MAP_H */

