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
#include <string>

using namespace std;

namespace World {
	
/*
 * A map is an 2 dimensions array of heights.
 * Each value is the height of a corner of a tile.
 */
class Map {
public:
	Map(int w, int l, float height_scale = 1.0f);
	~Map();
	bool render();		// Open Gl Rendering !
	
	bool readFile(string incoming);
	
private:
	Map(const Map& orig);
	void resetMap();

private:
	int width;
	int length;
	int scale_h;
	
	uint8_t* heights;	// height[w][h]
};

}

#endif /* MAP_H */

