//
// Simple .obj viewer(vertex only)
//
#include "Model.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#include <GLShader.hpp>
#include "TextureLoader.hpp"

#include <sys/time.h>

using namespace std;

vector<tinyobj::material_t> Model::materials;
map<string, GLuint> Model::textures;
map<const string, Model*> Model::models;

int width = 768;
int height = 768;

float curr_quat[4];
float prev_quat[4];
float eye[3], lookat[3], up[3];

GLFWwindow* window;

static string GetBaseDir(const string &filepath)
{
	if (filepath.find_last_of("/\\") != string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

static bool FileExists(const string &abs_filename)
{
	bool ret;
	FILE *fp = fopen(abs_filename.c_str(), "rb");
	if (fp)
	{
		ret = true;
		fclose(fp);
	}
	else
	{
		ret = false;
	}

	return ret;
}

static void CheckErrors(string desc)
{
	static string last_error;

	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		stringstream err;
		err << desc << ": (" << e << ')';
		if (err.str() != last_error)
		{
			last_error = err.str();
			cerr << "OpenGL error in " << last_error << endl;
		}
	}
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3])
{
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f)
	{
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
	}
}

Model::Model()
{
	// Append `default` material
	if (materials.size() == 0)
		materials.push_back(tinyobj::material_t());
}

Model::~Model()
{
	cerr << "deleting model" << endl;
}

void my_assert_sup(int i, int j)
{
	if (j < 0)
		cerr << "Model my_assert error " << i << ',' << j << endl;
	if (i <= j)
		cerr << "Model my_assert error " << i << ',' << j << endl;
	assert(i > j);
}

bool Model::loadObjAndConvert(const string& file, vector<Triangle>* triangles)
{
	filename = file;
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;

	string base_dir = GetBaseDir(file);
#ifdef _WIN32
	base_dir += "\\";
#else
	base_dir += "/";
#endif

	string err;
	bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str(), base_dir.c_str());

	if (!err.empty())
	{
		cerr << err << endl;
	}

	if (!ret)
	{
		cerr << "Failed to load " << file << endl;
		return false;
	}

#if 0
	cout << "MODEL " << file << endl;
	cout << "# of vertices  = " << (int)(attrib.vertices.size()) / 3 << endl;
	cout << "# of normals   = " << (int)(attrib.normals.size()) / 3 << endl;
	cout << "# of texcoords = " << (int)(attrib.texcoords.size()) / 2 << endl;
	cout << "# of materials = " << (int)materials.size() << endl;
	cout << "# of shapes    = " << (int)shapes.size() << endl;
#endif
	// Append `default` material
	materials.push_back(tinyobj::material_t());

	// Load diffuse textures
	{
		for (size_t m = 0; m < materials.size(); m++)
		{
			tinyobj::material_t* mp = &materials[m];

			if (mp->diffuse_texname.length() > 0)
			{
				// Only load the texture if it is not already loaded
				if (textures.find(mp->diffuse_texname) == textures.end())
				{
					GLuint texture_id;

					string texture_filename = mp->diffuse_texname;
					if (!FileExists(texture_filename))
					{
						// Append base dir.
						texture_filename = base_dir + mp->diffuse_texname;
						if (!FileExists(texture_filename))
						{
							cerr << "Unable to find file: " << mp->diffuse_texname << endl;
						}
					}

					glGenTextures(1, &texture_id);
					glBindTexture(GL_TEXTURE_2D, texture_id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					//TextureLoader::genGlCheckerBoard();
					TextureLoader::genGlTexture(texture_filename);
					glBindTexture(GL_TEXTURE_2D, 0);
					// stbi_image_free(image);
					textures.insert(make_pair(mp->diffuse_texname, texture_id));
				}
			}
		}
	}

	bmin[0] = bmin[1] = bmin[2] = numeric_limits<float>::max();
	bmax[0] = bmax[1] = bmax[2] = -numeric_limits<float>::max();

	{
		cout << "Shape size " << shapes.size() << endl;
		for (size_t s = 0; s < shapes.size(); s++)
		{
			DrawObject o;
			vector<float> vb; // pos(3float), normal(3float), color(3float)
			cout << "shapes[" << s << "].mesh.indices.size()=" << shapes[s].mesh.indices.size() << endl;

			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
			{
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				int current_material_id = shapes[s].mesh.material_ids[f];

				if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size())))
				{
					// Invaid material ID. Use default material.
					current_material_id = materials.size() - 1; // Default material is added to the last item in `materials`.
				}
				//if (current_material_id >= materials.size()) {
				//    cerr << "Invalid material index: " << current_material_id << endl;
				//}
				//
				float diffuse[3];
				for (size_t i = 0; i < 3; i++)
				{
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				float tc[3][2];
				if (attrib.texcoords.size() > 0)
				{
					my_assert_sup(attrib.texcoords.size(), 2 * idx0.texcoord_index + 1);
					my_assert_sup(attrib.texcoords.size(), 2 * idx1.texcoord_index + 1);
					my_assert_sup(attrib.texcoords.size(), 2 * idx2.texcoord_index + 1);

					tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
					tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
					tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
					tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
					tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
					tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];

				}
				else
				{
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++)
				{
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
					bmin[k] = min(v[0][k], bmin[k]);
					bmin[k] = min(v[1][k], bmin[k]);
					bmin[k] = min(v[2][k], bmin[k]);
					bmax[k] = max(v[0][k], bmax[k]);
					bmax[k] = max(v[1][k], bmax[k]);
					bmax[k] = max(v[2][k], bmax[k]);
				}

				float n[3][3];
				if (attrib.normals.size() > 0)
				{
					int f0 = idx0.normal_index;
					int f1 = idx1.normal_index;
					int f2 = idx2.normal_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);
					for (int k = 0; k < 3; k++)
					{
						n[0][k] = attrib.normals[3 * f0 + k];
						n[1][k] = attrib.normals[3 * f1 + k];
						n[2][k] = attrib.normals[3 * f2 + k];
					}
				}
				else
				{
					// compute geometric normal
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}

				float gx=0,gy=0,gz=0;

				for (int k = 0; k < 3; k++)
				{
					// VERTEX
					vb.push_back(v[k][0]);
					vb.push_back(v[k][1]);
					vb.push_back(v[k][2]);

					gx += v[k][0];
					gy += v[k][1];
					gz += v[k][2];

					// NORMAL
					vb.push_back(n[k][0]);
					vb.push_back(n[k][1]);
					vb.push_back(n[k][2]);

					// Combine normal and diffuse to get color.
					if (false)
					{
						float c[3];
						float normal_factor = 0.2;
						float diffuse_factor = 1 - normal_factor;

						// color
						c[0] = n[k][0] * normal_factor + diffuse[0] * diffuse_factor;
						c[1] = n[k][1] * normal_factor + diffuse[1] * diffuse_factor;
						c[2] = n[k][2] * normal_factor + diffuse[2] * diffuse_factor;

						// Normalize color
						float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
						if (len2 > 0.0f)
						{
							float len = sqrtf(len2);

							c[0] /= len;
							c[1] /= len;
							c[2] /= len;
						}
						vb.push_back(c[0] * 0.5 + 0.5);
						vb.push_back(c[1] * 0.5 + 0.5);
						vb.push_back(c[2] * 0.5 + 0.5);
					}
					else
					{
						// COLOR
						if (false)
						{
							vb.push_back(1);
							vb.push_back(1);
							vb.push_back(0);
						}
						else
						{
							vb.push_back(materials[current_material_id].diffuse[0]);
							vb.push_back(materials[current_material_id].diffuse[1]);
							vb.push_back(materials[current_material_id].diffuse[2]);
						}
					}

					// ?????? texture coords
					vb.push_back(tc[k][0]);
					vb.push_back(tc[k][1]);
				}

				gx /= 3.0;
				gy /= 3.0;
				gz /= 3.0;
				normals.push_back(gx);
				normals.push_back(gy);
				normals.push_back(gz);

				int kk=0;
				normals.push_back(gx+n[kk][0]);
				normals.push_back(gy+n[kk][1]);
				normals.push_back(gz+n[kk][2]);
			}

			cout << "Normals " << normals.size() << endl;

			o.vb = 0;
			o.numTriangles = 0;

			// OpenGL viewer does not support texturing with per-face material.
			if (shapes[s].mesh.material_ids.size() > 0 && shapes[s].mesh.material_ids.size() > s)
			{
				// Base case
				o.material_id = shapes[s].mesh.material_ids[s];
			}
			else
			{
				o.material_id = materials.size() - 1; // = ID for default material.
			}

			if (vb.size() > 0)
			{
				glGenBuffersARB(1, &o.vb);
				glBindBufferARB(GL_ARRAY_BUFFER, o.vb);
				glBufferDataARB(GL_ARRAY_BUFFER, vb.size() * sizeof (float), &vb.at(0),
							GL_STATIC_DRAW);
				o.numTriangles = vb.size() / ((3 + 3 + 3 + 2) * 3);
				cout << "shape[" << s << "], triangles: " << o.numTriangles << endl;
			}

			drawObjects.push_back(o);
		}
	}

	// printf("bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
	// printf("bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

	return true;
}
static const GLsizei stride = (3 + 3 + 3 + 2) * sizeof (float);

void Model::renderBoundingBox() const
{
	glBegin(GL_LINE_LOOP);
	glVertex3f(bmin[0], bmin[1], bmin[2]);
	glVertex3f(bmax[0], bmin[1], bmin[2]);
	glVertex3f(bmax[0], bmax[1], bmin[2]);
	glVertex3f(bmin[0], bmax[1], bmin[2]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(bmin[0], bmin[1], bmax[2]);
	glVertex3f(bmax[0], bmin[1], bmax[2]);
	glVertex3f(bmax[0], bmax[1], bmax[2]);
	glVertex3f(bmin[0], bmax[1], bmax[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(bmin[0], bmin[1], bmin[2]);
	glVertex3f(bmin[0], bmin[1], bmax[2]);
	glVertex3f(bmax[0], bmin[1], bmin[2]);
	glVertex3f(bmax[0], bmin[1], bmax[2]);
	glVertex3f(bmax[0], bmax[1], bmin[2]);
	glVertex3f(bmax[0], bmax[1], bmax[2]);
	glVertex3f(bmin[0], bmax[1], bmin[2]);
	glVertex3f(bmin[0], bmax[1], bmax[2]);
	glEnd();
}

void Model::render(bool bnormals) const
{
	glMatrixMode(GL_MODELVIEW);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// glEnable(GL_POLYGON_OFFSET_FILL);
	// glPolygonOffset(1.0, 1.0);

	for (size_t i = 0; i < drawObjects.size(); i++)
	{
		GLuint shader = 0;
		const DrawObject& o = drawObjects[i];
		if (o.vb < 1)
			continue;

		glBindBufferARB(GL_ARRAY_BUFFER, o.vb);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if ((o.material_id < materials.size()))
		{
			tinyobj::material_t & material(materials[o.material_id]);
			string diffuse_texname = material.diffuse_texname;
			if (textures.find(diffuse_texname) != textures.end())
			{
				glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
			}

			if (material.shininess)
			{
				if (shader == 0)
				{
					CheckErrors("before Shininess shader loading");
					shader = GLShader::loadGlsl("phong");
					if (shader)
						glUseProgram(shader);
					CheckErrors("Shininess shader loading fail");
				}
			}
		}

		glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
		glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof (float) * 3));
		glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof (float) * 6));
		glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof (float) * 9));
		glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);

		if (bnormals)
		{
			auto it = normals.begin();
			while(it != normals.end())
			{
				float x,y,z;

				x=*it++;
				y=*it++;
				z=*it++;
				glVertex3f(x, y, z);

				x=*it++;
				y=*it++;
				z=*it++;
				glVertex3f(x, y, z);
			}
		}

		glEnd();

		CheckErrors("Model::draw");
		glBindTexture(GL_TEXTURE_2D, 0);
		if (shader) glUseProgram(0);
	}
}

void Model::renderWireFrame() const
{
	// draw wireframe
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	glColor3f(0.0f, 0.0f, 0.4f);
	for (size_t i = 0; i < drawObjects.size(); i++)
	{
		const DrawObject &o = drawObjects[i];
		if (o.vb < 1)
		{
			continue;
		}

		glBindBuffer(GL_ARRAY_BUFFER, o.vb);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
		glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof (float) * 3));
		glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof (float) * 6));
		glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof (float) * 9));

		glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
		CheckErrors("drawarrays");
	}
}

void Model::main()
{

	if (false == loadObjAndConvert("test"))
	{
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// camera & rotate
	render();
}

const Model* Model::get(const string& name, bool reload)
{
	if (!reload)
		if (models.find(name) != models.end())
			return models[name];

	string file = "data/models/" + name + ".obj";
	Model* model = new Model;
	if (model->loadObjAndConvert(file))
		models[name] = model;
	else
	{
		delete model;
		model = 0;
		if (reload)
		{
			if (models.find(name) != models.end())
				model = models[name];
		}
	}
	return model;
}

