#include "GLShader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

map<string, GLuint> GLShader::shaders;

std::string GLShader::readFile(string filePath)
{
	string part = "";
	bool in_part = true;
	bool bpart = false;

	filePath = "data/shaders/" + filePath;

	string::size_type diese = filePath.find('#');
	std::string content;

	if (diese != string::npos)
	{
		bpart = true;
		in_part = false;
		part = '[' + filePath.substr(diese + 1) + ']';
		filePath.erase(diese);
	}

	std::ifstream fileStream(filePath.c_str(), std::ios::in);

	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		if (bpart && line.substr(0, part.length()) == part)
		{
			in_part = true;
		}
		else if (bpart && line[0] == '[')
		{
			in_part = false;
		}
		else if (in_part)
			content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint GLShader::loadGlsl(const string& glsl_file)
{
	string realglsl_file = glsl_file + ".glsl";
	string vertex = realglsl_file + "#VertexShader";
	string fragment = realglsl_file + "#FragmentShader";
	return load(vertex, fragment, glsl_file);
}

GLuint GLShader::load(string vertex_path, string fragment_path, string key)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShadeModel(GL_SMOOTH);
	if (key.length() == 0)
		key = vertex_path + '_' + fragment_path;

	if (vertex_path.find('#') == string::npos)
	{
		if (fragment_path.length() == 0)
			fragment_path = vertex_path + ".frag";
		else
			fragment_path = vertex_path + ".frag";
		vertex_path = vertex_path + ".vert";
	}
	else if (key.length() == 0)
		key = vertex_path.substr(0, vertex_path.find('#'));

	if (shaders.find(key) != shaders.end())
		return shaders[key];

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader " << key << vertex_path << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	if (vertShaderError.size() > 1) std::cout << &vertShaderError[0] << std::endl;

	// Compile fragment shader
	std::cout << "Compiling fragment shader " << key << fragment_path << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	if (fragShaderError.size() > 1) std::cout << &fragShaderError[0] << std::endl;

	std::cout << "Linking program " << key << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError( (logLength > 1) ? logLength : 1 );
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	if (programError.size() > 1)
	{
		std::cout << &programError[0] << std::endl;
		program = 0;
	}
	else
	{
		shaders[key] = program;
		if (program == 0)
			cerr << "Bad to return 0 on error, we had a real program with ID 0 !" << endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}