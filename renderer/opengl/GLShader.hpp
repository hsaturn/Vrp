#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

using namespace std;

class GLShader
{
	public:
		
		static string readFile(string filePath);
		
		/**
		 * 
		 * @param vertex_file ex: phong
		 * @param fragment_file, empty or name of fragment
		 * @param key may be left empty (key used to store the shader)
		 * @return 0 or program id
		 */
		static GLuint load(string vertex_file, string fragment_file="", string key="");
		
		/**
		 * @param glsl_file, [VertexShader] ... [FragmentShader] ... file
		 * @return 0 or program id
		 */
		static GLuint loadGlsl(const string& glsl_file);
		
	private:
		static map<string, GLuint> shaders;	// map vertex_fragment => program
};

