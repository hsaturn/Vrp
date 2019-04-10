/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Triangle.cpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:34
 */

#include "Triangle.hpp"

result hplane::intersect(const Triangle& t)
{
   result r;

   bool aup = t.a.z > z;
   bool bup = t.b.z > z;
   bool cup = t.c.z > z;

   if (t.a.z==z && t.b.z==z && t.c.z==z)
   {
      r.state = result::full;
      return r;
   }

   if (aup==bup && bup==cup) return r; // Either triangle over or under the plane

   // one point of the triangle is on the plane, the two others are the same side of the plane: no result
   if (t.a.z==z && bup==cup) return r;
   if (t.b.z==z && aup==cup) return r;
   if (t.c.z==z && aup==bup) return r;

   // at this point, we have an intersection
   r.state=result::segment;

   // two points are on the plane
   if (t.a.z==z && t.b.z==z)
   {
      r.a=t.a;
      r.b=t.b;
      return r;
   }

   if (t.a.z==z && t.c.z==z)
   {
      r.a=t.a;
      r.b=t.c;
      return r;
   }

   if (t.b.z==z && t.c.z==z)
   {
      r.a=t.b;
      r.b=t.c;
      return r;
   }

   // general intersection, compute it
   if (aup==bup)  // c is at the opposite side of a & b
   {
      r.a=intersect_ray(t.c, t.a);
      r.b=intersect_ray(t.c, t.b);
   }
   else if (aup==cup)  // b is at the opposite side of a & c
   {
      r.a=intersect_ray(t.b, t.a);
      r.b=intersect_ray(t.b, t.c);
   }
   else
   {
      assert(bup==cup); // a is at the opposite side of c & b
      r.a=intersect_ray(t.a, t.b);
      r.b=intersect_ray(t.a, t.c);
   }
   return r;
}

point hplane::intersect_ray(const point& a, const point& b)
{
   assert(!(a.z==z && b.z==z));  // Not both on the plane
   assert((a.z>=z && b.z<=z) || (a.z<=z && b.z>=z));   // Not at the same side

   float k=(b.z-z)/(b.z-a.z);
   return point(
      b.x-(b.x-a.x)*k,
      b.y-(b.y-a.y)*k,
      z);
}



