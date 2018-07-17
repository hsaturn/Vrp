/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Triangle.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:34
 */

#ifndef TRIANGLE_HPP
#    define TRIANGLE_HPP
#include "glm/glm.hpp"


class Triangle
{
  public:
	Triangle();
	Triangle(const Triangle& orig);
	virtual ~Triangle();
	
  private:
	glm::vec3 m1,m2,m3;
	glm::vec3 n1,n2,n3;
};

#endif /* TRIANGLE_HPP */

