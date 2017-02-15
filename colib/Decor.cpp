#include "Decor.hpp"
#include <GL/glew.h>
#include <model/TextureLoader.hpp>
#include <iostream>
#include <Color.h>

using namespace std;

void Decor::render(float size_x, float pos_y, float size_z, float tile_size)
{
	static bool init=false;
	static GLuint floor_tex;
	
	if (init==false)
	{
		init = true;
		glGenTextures(1, &floor_tex);
		glBindTexture(GL_TEXTURE_2D, floor_tex);
		TextureLoader::genGlCheckerBoard();
	}
	else
		glBindTexture(GL_TEXTURE_2D, floor_tex);
	
	glPushMatrix();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	const float size_x_tex = size_x / tile_size / 2.0;
	const float size_z_tex = size_z / tile_size / 2.0;
	
	glTranslatef(-size_x/2, 0, -size_z/2);
	Color::gray.render();
	glBegin(GL_QUADS);
	
	glTexCoord2f(0,0);
	glVertex3f(0, 0, 0);
	
	glTexCoord2f(0.0f, size_z_tex);
	glVertex3f(0, 0, size_z);
	
	glTexCoord2f(size_x_tex, size_z_tex);
	glVertex3f(size_x, 0,  size_z);
	
	glTexCoord2f(size_x_tex, 0);
	glVertex3f(size_x, 0, 0);
	
	glEnd();

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}