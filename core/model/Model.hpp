/* 
 * File:   Model.hpp
 * Author: hsaturn
 *
 * Created on 3 février 2017, 00:44
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "tiny_obj_loader.h"
#include <glm.hpp>
#include "Triangle.hpp"

using namespace std;

class Model
{
	public:
		
		static const Model* get(const string& name, bool reload=false, vector<Triangle> *triangles=nullptr);
		void renderWireFrame() const;
		void render(bool draw_normals=false) const;
		void renderBoundingBox() const;
		
		// Bounding box
		const glm::vec3& getMinCoord() const { return bmin; }
		const glm::vec3& getMaxCoord() const { return bmax; }
		
		float getLengthX() const { return bmax[0]-bmin[0]; }
		float getHeight()  const { return bmax[1]-bmin[1]; }
		float getLengthZ() const { return bmax[2]-bmin[2]; }
		
		const string& getFileName() const { return filename; }
      void setShortName(const string& name) { msShortName = name; }
      const string& getShortName() const { return msShortName; }

	private:
		Model();
		~Model();
		Model(const Model&);
		Model& operator=(const Model&);
		
		bool loadObjAndConvert(const string&, vector<Triangle>* triangles=0);
      
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
		string filename;
		std::vector<float> normals;
		
	private: // Static members
		static std::vector<tinyobj::material_t> materials;
		static std::map<std::string, GLuint> textures;
		static map<const string, Model*>	models;
      string msShortName;
		
		void main();
		
};
