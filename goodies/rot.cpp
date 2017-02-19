// Working OpenGL3.3 Context with GLFW Window showing a colored rectangle with Kitten 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <fstream>

int main () {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow (640, 480, "2 Triangles making a square", NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent (window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	// get OpenGl version info to check video card on PC or Laptop
	// const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	//const GLubyte* version = glGetString (GL_VERSION); // version as a string
	//printf ("Renderer: %s\n", renderer);
	//printf ("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	// Create float array of vertices, 
	float vertices[] = {
		// Position Color TexCoords
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Vertex 1: Top Left Red 
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Vertex 2: Top Right Blue 
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Vertex 3: Bottom Right Green 
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // vertex 4: Bottom Left White

	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0

	};

	//Create Vertex Buffer Objects VBO
	GLuint vbo = 0;
	glGenBuffers (1, &vbo); // Generate Buffer
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	const char* vertex_shader = 
		"#version 330\n"
		"in vec2 position;"
		"in vec2 texcoord;" 
		"in vec3 color;"

		"out vec3 Color;"
		"out vec2 Texcoord;"
		"uniform mat4 trans;"
		"void main() {"
		"Color = color;"
		"Texcoord = texcoord;"
		" gl_Position = trans * vec4(position, 0.0, 1.0);"

		"}";

	const char* fragment_shader =
		"#version 330\n"
		"in vec3 Color;"
		"in vec2 Texcoord;"
		"out vec4 outColor;"
		// "out vec4 frag2_colour;"
		"uniform sampler2D tex;"
		"void main() {"
		" outColor = texture(tex, Texcoord)* vec4(Color, 1.0);" //vec4(1.0, 1.0, 0.5, 1.0);" // changing vec4 numbers changes RGB color to be used
		// " frag2_colour = vec4(colour2, 1.0);"
		"}";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL); // learn how to use external shader text files
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	GLuint tex;
	glGenTextures(1, &tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	int width = 2, height = 2;
	unsigned char* image =
		SOIL_load_image("Images/kitten2.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

	while (!glfwWindowShouldClose (window)) {
		//Wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate the transformation
		// glm::mat4 trans;
		glm::mat4 trans = glm::translate(
				// (float)clock() / (float)CLOCKS_PER_SEC / 0.5f),

			glm::mat4(1.0f),
			glm::vec3(0.5f, 0.0f, 0.0f)

				);

		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr( trans));

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		//Draw points 0-3 from the currently bound VAO with current in_use shader
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Update other events like input handling
		glfwPollEvents();
		//Put the stuff we've been drawing onto the display screen
		glfwSwapBuffers(window);
		if (glfwGetKey (window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (window, 1);
		}

		else
			if (glfwGetKey (window, GLFW_KEY_SPACE)) {

				glm::mat4 trans = glm::translate(

						glm::mat4(1.0f),
						glm::vec3(0.5f, 0.0f, 0.0f)

						);
				return 0;

			}

	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
