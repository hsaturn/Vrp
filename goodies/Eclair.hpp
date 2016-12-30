/* 
 * File:   Eclair.hpp
 * Author: hsaturn
 *
 * Created on 20 novembre 2015, 02:19
 */

#ifndef ECLAIR_HPP
#define	ECLAIR_HPP

#include "Object.h"

using namespace std;
 
class Eclair
{
    public:
    Eclair(Vector3D b,Vector3D e, double width);
    ~Eclair();
    void blit3D(Vector3D cam);
	virtual void render(bool resetTimer);
 
 
    Vector3D begin,end;
    vector<Vector3D> begins,ends;
    vector<Vector3D> poss;
    vector<vector<Vector3D> > possr;
    vector<int> beginsram;
    vector<Vector3D> rams;
    double w;
};


#endif	/* ECLAIR_HPP */

