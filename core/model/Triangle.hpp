/* 
 * File:   Triangle.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:34
 */

#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include <math.h>

using namespace std;
template<typename t>
t sqr(t value) { return value*value; }

class point
{
public:
   point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
   point() {}

   float x,y,z;

   friend ostream& operator<< (ostream& out, const point& p)
   {
      out << '[' << p.x << "," << p.y << "," << p.z << ']';
      return out;
   }

   float man_distance(const point& p)
   {
      return abs(x-p.x)+abs(y-p.y)+abs(z-p.z);
   }

   float sqr_distance(const point& p)
   {
      return sqr(x-p.x)+sqr(y-p.y)+sqr(z-p.z);
   }

   float distance(const point& p)
   {
      return sqrtf(sqr_distance(p));
   }
};

class Normal
{
   public:
      Normal()=default;
      Normal(glm::vec3 _start, glm::vec3 _norm)
      : start(_start), norm(_norm)
      {
      }

      glm::vec3 start;
      glm::vec3 norm;
};

class Triangle
{
public:
   Triangle() = default;
   Triangle(const point& a_, const point& b_, const point& c_)
           : a(a_), b(b_), c(c_) {}

   point a,b,c;
   
   friend ostream& operator<< (ostream& out, const Triangle& t)
   {
      out << "T(" << t.a << t.b << t.c << ')';
      return out;
   }
   
   Normal normal;
};

class result
{
public:
   enum estate : char
   {
      empty,
      full,
      segment
   };

   result() : state(empty){};

   point a;
   point b;


   friend ostream& operator<< (ostream& out, const result& r)
   {
      if (r.state == empty)
      {
         out << "{0}";
         return out;
      }
      if (r.state == full)
      {
         out << "{*}";
         return out;
      }
      out << '{' << r.a << "," << r.b << '}';
      return out;
   }

   operator bool() const { return state!=empty; }
   estate state;
};


class hplane
{
public:
   hplane(float z_) : z(z_) {}
   float z;

   friend ostream& operator<< (ostream& out, const hplane& p)
   {
      out << "P(" << p.z << ')';
      return out;
   }
   result intersect(const Triangle& t);

   // a & b must not be both on the plane
   // a & b must no be the same side of the plane
   point intersect_ray(const point& a, const point& b);

};

/*
float frand()
{
   return (float)rand() / (float) RAND_MAX;
}
int main(int argc, const char* argv[])
{
   hplane p(5);
   triangle t(
      {10,0,5},
      {0,10,5},
      {5,5,5}
      );

   cout << t << endl;
   cout << "I= " << p.intersect(t) << endl;
   float dist;

   for(long l=0; l<10000000; l++)
   {
      point p1{frand(), frand(), frand()};   // 240ms
      point p2{frand(), frand(), frand()};   // 240ms
      dist += p1.sqr_distance(p2);      // man/sqr_distance: 120ms distance: 20ms
   }

   cout << dist << endl;

	return 0;
}



*/