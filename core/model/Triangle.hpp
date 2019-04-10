/* 
 * File:   Triangle.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:34
 */

#pragma once
#include "glm/glm.hpp"

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
      Triangle(const Triangle& orig) = default;
      virtual ~Triangle() = default;

      glm::vec3& a() { return m1; }
      glm::vec3& b() { return m2; }
      glm::vec3& c() { return m3; }
      Normal& normal() { return m_normal; }
	
   private:
      glm::vec3 m1,m2,m3;  // Points
      Normal m_normal;
};

