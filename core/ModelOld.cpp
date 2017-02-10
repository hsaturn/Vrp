#include "ModelOld.hpp"
#include <GL/glew.h>
#include <iostream>
#include "objloader.hpp"

map<const string, ModelOld*> ModelOld::models;

ModelOld::ModelOld()
{
}

ModelOld::~ModelOld()
{
	glDeleteBuffers(1, &vbo_id);
}

void ModelOld::render() const
{

		//Vertices of a triangle (counter-clockwise winding)
		//float data[] = {1.0, 0.0, 1.0, 0.0, 0.0, -1.0, -1.0, 0.0, 1.0};

		//Create a new VBO and use the variable id to store the VBO id

		//Upload vertex data to the video device
		//glBufferData(GL_ARRAY_BUFFER, sizeof(data), &vertices[0], GL_STATIC_DRAW);

		//Make the new VBO active. Repeat here incase changed since initialisation
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

		//Draw Triangle from VBO - do each time window, view point or data changes
		//Establish its 3 coordinates per vertex with zero stride in this array; necessary here
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		//Establish array contains vertices (not normals, colours, texture coords etc)
		glEnableClientState(GL_VERTEX_ARRAY);

		//Actually draw the triangle, giving the number of vertices provided
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		return;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ModelOld::load(const string& name)
{
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	
	bool loaded = loadOBJ(name.c_str(),
		vertices, uvs, normals);
	if (loaded)
	{
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(glm::vec3),
			&vertices[0],
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return loaded;
}

const ModelOld* ModelOld::get(const string& name)
{
	if (models.find(name) != models.end())
		return models[name];
	
	string file="models/"+name+".obj";
	ModelOld* model = new ModelOld;
	if (model->load(file))
	{
		models[name] = model;
	}
	else
	{
		delete model;
		model=0;
	}
	return model;
}