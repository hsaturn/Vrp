/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.hpp
 * Author: hsaturn
 *
 * Created on 3 février 2017, 00:44
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace std;

class Model
{
	public:
		
		static const Model* get(const string& name);
		void render() const;
		
	private:
		Model();
		~Model();
		Model(const Model&);
		Model& operator=(const Model&);
		
		bool load(const string&);
		
		std::vector< glm::vec3 > vertices;
		
		static map<const string, Model*>	models;
		GLuint vbo_id;
		
};

#endif /* MODEL_HPP */

