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
#include "tiny_obj_loader.h"
#include <glm.hpp>

using namespace std;

class Model
{

	
	public:
		
		static const Model* get(const string& name);
		void renderWireFrame() const;
		void render() const;
		void renderBoundingBox() const;
		
		// Bounding box
		const glm::vec3& getMinCoord() const { return bmin; }
		const glm::vec3& getMaxCoord() const { return bmax; }
		
		float getLengthX() const { return bmax[0]-bmin[0]; }
		float getHeight()  const { return bmax[1]-bmin[1]; }
		float getLengthZ() const { return bmax[2]-bmin[2]; }
		
	private:
		Model();
		~Model();
		Model(const Model&);
		Model& operator=(const Model&);
		
		bool loadObjAndConvert(const string&);

		typedef struct {
		  GLuint vb;  // vertex buffer FIXME opengl memory leak
		  int numTriangles;
		  size_t material_id;
		} DrawObject;
		
		/*
		std::vector< glm::vec3 > vertices;
		
		static map<const string, Model*>	models;
		GLuint vbo_id;
		*/
		
		glm::vec3 bmin;
		glm::vec3 bmax;
		std::vector<DrawObject> drawObjects;
		
	private: // Static members
		static std::vector<tinyobj::material_t> materials;
		static std::map<std::string, GLuint> textures;
		static map<const string, Model*>	models;
		
		void main();
		
};

#endif /* MODEL_HPP */

