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

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

using namespace std;

class ModelOld
{
	public:
		
		static const ModelOld* get(const string& name);
		void render() const;
		
	private:
		ModelOld();
		~ModelOld();
		ModelOld(const ModelOld&);
		ModelOld& operator=(const ModelOld&);
		
		bool load(const string&);
		
		std::vector< glm::vec3 > vertices;
		
		static map<const string, ModelOld*>	models;
		GLuint vbo_id;
		
};
