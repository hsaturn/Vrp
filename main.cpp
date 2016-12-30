#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>
#include <Cube/Cube.h>
#include <Server.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <string>
#include <list>
#include <GL/glew.h>
#include <GL/glut.h>
#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "imageloader.h"
#include "Random.hpp"
#include "texture.hpp"
#include "Vrp/Vrp.hpp"
#include <ansi_colors.hpp>

#include "StringUtil.hpp"
#include "Background.h"
#include "Help.h"
#include <Widget.h>

int SCREEN_WIDTH = 200;
int SCREEN_HEIGHT = 200;

BackgroundStars background;

using namespace std;
using namespace hwidgets;
string lastState = "";

bool redisplayAsked = false;
bool backward = true;

long lastUpdateWidget = 0;

struct thr_info
{
	thread* thr;
	string cmd;
};

list<thr_info*> exec_list;

float back_x = 4.1;
float back_y = 4.1;
float back_z = 2.1;
float back_s = 0.5;
using namespace std;
GLint cube_server;
bool bAxis = false;
Server* server;
char presse;
double x, y, xold, yold;
GLuint program;
double anglex = 0;
double angley = 0;
double anglez = 0;
list<string> cmdQueue;
glm::mat4 orient;
float _anglex = 0;
float _angley = 0;
float animx = 0;
float animy = 0;
float i;
int k = 0;
const float BOX_SIZE = 2.0f;
GLuint white_textureId = 0;
GLuint red_textureId;
GLuint blue_textureId;
GLuint green_textureId;
GLuint yellow_textureId;
GLuint orange_textureId;
GLuint _displayListId_smallcube; //The OpenGL id of the display list
//Makes the image into a texture, and returns the id of the texture
long sizex = 600;
long sizey = 600;
bool resetTimer = true;

const float flatx_org = 2.5;
const float flaty_org = 4.8;
const float flatz_org = 4.0;
const float flats_org = 1.0;
bool flat = true;
float flatx = flatx_org;
float flaty = flaty_org;
float flatz = flatz_org;
float flats = flats_org;

const float cubex_org = 0.0;
const float cubey_org = -0.8;
const float cubez_org = 0.0;
const float scale_org = 2.0;
float cubex = cubex_org;
float cubey = cubey_org;
float cubez = cubez_org;
float scale = scale_org;

const float hudr_org = 1.0;
const float hudv_org = 1.0;
const float hudb_org = 0.0;
bool hud = true;
float hudr = hudr_org;
float hudv = hudv_org;
float hudb = hudb_org;

map<string, string> patterns;
map<string, string> macros;

Cube* getCube()
{
	return (Cube*) ObjectBuilder::getInstance("cube");
}

#define cube getCube()

template <class T>
string toString(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

void trim(string& s, bool bWithCr = true)
{
	while (s.length() > 0 && (s[0] == ' ' || (bWithCr && (s[0] == 13 || s[0] == 10))))
		s.erase(0, 1);
	while (s.length() > 0 && (s[s.length() - 1] == ' ' || (bWithCr && (s[s.length() - 1] == 13 || s[s.length() - 1] == 10))))
		s.erase(s.length() - 1, 1);
}

void exec(string& cmd)
{
	trim(cmd);
	if (cmd.length())
	{
		try
		{
			bool bBackGround = cmd[cmd.length() - 1] == '&';
			if (bBackGround) cmd.erase(cmd.length() - 1, 1);
			thr_info* t = new thr_info();
			t->cmd = cmd;
			t->thr = new thread(
								[cmd]()
								  {
									system(cmd.c_str());
								server->send("#EVENT exec end " + cmd);
								  });
			exec_list.push_back(t);
			if (!bBackGround)
			{
				t->thr->join();
			}
		}
		catch (std::exception &e)
		{
			cerr << "THREAD " << e.what() << endl;
		}
	}
}

bool run(const string& sFileName)
{
	bool ret = false;
	ifstream queue(sFileName);
	if (queue.is_open())
	{
		ret = true;
		while (queue.good())
		{
			string cmd;
			getline(queue, cmd);
			cmdQueue.push_back(cmd);
		}
		cmdQueue.push_back("echo END OF SCRIPT: " + sFileName);
	}
	return ret;
}

void fillPatterns();

void reboot()
{
	macros.clear();
	patterns.clear();
	Widget::clear();

	run("startup.cub");
	fillPatterns();
	run("patterns.cub");
	run("macros.cub");
}

void drawBackground2()
{
	static GLint tt = 0;
	if (tt == 0)
	{
		Image* image = loadBMP("background.bmp");
		tt = loadTexture(image);
		delete image;
	}
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho(0, 1, 0, 1, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// No depth buffer writes for background.
	glDepthMask(false);

	glBindTexture(GL_TEXTURE_2D, tt);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
		glTexCoord2f(0, 1);
		glVertex2f(0, 1);
		glTexCoord2f(1, 1);
		glVertex2f(1, 1);
		glTexCoord2f(1, 0);
		glVertex2f(1, 0);
	}
	glEnd();

	glDepthMask(true);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawBackground()
{
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, 0, 1);
	if (white_textureId == 0)
	{
		Image* image = loadBMP("background.bmp");
		white_textureId = loadTexture(image);
		delete image;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, white_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(10.0f, -10.0f);
	glEnd();

	glPopMatrix();
}

string getWord(string& s, const string &sSeparators)
{
	string sWord;
	trim(s);

	if (s.length())
	{
		string::size_type iPos = string::npos;

		for (string::size_type i = 0; i < sSeparators.length(); i++)
		{
			string::size_type i1 = s.find(sSeparators[i]);

			if (i1 != string::npos)
			{
				if (iPos == string::npos || iPos > i1)
				{
					iPos = i1;
				}
			}
		}

		if (iPos != string::npos)
		{
			sWord = s.substr(0, iPos);
			s.erase(0, iPos + 1);
			trim(s);
		}
		else
		{
			sWord = s;
			s = "";
		}
	}
	return sWord;
}

void handleKeypress(unsigned char key, int x, int y)
{
	static string cmd;

	Widget::handleKeypress(key, x, y);
}

void handleResize(int w, int h)
{
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
	hwidgets::Widget::handleResize(w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double) w / (double) h, 1.0, 200);
	gluLookAt(0.0f, 5.5f, 15.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

	/* if (w <= h)
	   glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
	   2.0 * (GLfloat) h / (GLfloat) w, -1.0, 1.0);
	   else
	   glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
	   2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -1.0, 1.0);
	 */
	/* set matrix mode to modelview */
	glMatrixMode(GL_MODELVIEW);

}

void draw_smallcube()
{
	return;
	Image* image = loadBMP("white.bmp");
	white_textureId = loadTexture(image);
	delete image;

	Image* image1 = loadBMP("red.bmp");
	red_textureId = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("blue.bmp");
	blue_textureId = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("orange.bmp");
	orange_textureId = loadTexture(image3);
	delete image3;

	Image* image4 = loadBMP("green.bmp");
	green_textureId = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("yellow.bmp");
	yellow_textureId = loadTexture(image5);
	delete image5;


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, orange_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);


	//Top face
	//glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	glEnd();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, red_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Bottom face

	//glNormal3f(0.0, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, green_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);

	//Left face
	//glNormal3f(-1.0, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, blue_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);

	//Right face
	//glNormal3f(1.0, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, white_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);

	//Front face
	//glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);

	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, yellow_textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	//Back face
	//glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);

	glEnd();
	//glDisable(GL_TEXTURE_2D);

	//glutSwapBuffers();
}

void initRendering()
{
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);*/

	//	float lpos[] = { 50, 50, 50, 0 };
	// glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return;

	//Set up a display list for drawing a cube
	_displayListId_smallcube = glGenLists(1); //Make room for the display list
	glNewList(_displayListId_smallcube, GL_COMPILE); //Begin the display list
	draw_smallcube(); //Add commands for drawing a black area to the display list
	glEndList(); //End the display list


}

Face::Dir getDir(string& incoming, string& dir)
{
	dir = getWord(incoming);
	if (dir == "top") return Face::TOP;
	if (dir == "bottom") return Face::BOTTOM;
	if (dir == "down") return Face::BOTTOM;
	if (dir == "up") return Face::TOP;
	if (dir == "left") return Face::LEFT;
	if (dir == "right") return Face::RIGHT;
	if (dir == "front") return Face::FRONT;
	if (dir == "back") return Face::BACK;

	return Face::NONE;
}

float getFloat(string& incoming)
{
	string f = getWord(incoming);

	if (f.length() > 0 && ((f[0] >= '0' && f[0] <= '9') || f[0] == '.' || f[0] == '-'))
	{
		return atof(f.c_str());
	}
	return 0.0;
}

const Color* getColor(string& incoming)
{
	string org(incoming);
	const Color* color = Color::factory(incoming);

	if (color == 0)
		server->send("#ERROR Unknown color [" + org + "]");

	return color;
}

void onclose()
{
	delete server;
	server = 0;
	for (auto it = exec_list.begin(); it != exec_list.end(); it++)
	{
		(*it)->thr->join();
	}
}

void drawText(const char * message)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	//glRasterPos2f((GLfloat)0, (GLfloat)-400);
	//glRasterPos2f(0,0);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		//glutStrokeCharacter(GLUT_STROKE_ROMAN,*message);
		message++;
	}
}

void update(int value);
void drawScene();

void widgetUpdate(int value)
{
	lastUpdateWidget = 0;
	drawScene();
}

void drawHud(bool a)
{
	const float g_rotate = 0; // rotation du texte
	Color c(hudr, hudv, hudb);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	string state;

	if (cube)
	{
		if (!cube->isValid())
		{
			c = Color::red;
			state = "invalid";
		}
		else
			state = "scrambled";
		if (cube->isMade())
		{
			c = Color::green;
			state = "solved";
		}
		if (lastState != state)
		{
			Widget::pushEvent("cube", state);
			lastState = state;
		}
	}

	if (hud)
	{
		glBegin(GL_QUADS);
		c.render(0.1);
		glVertex2f(0, SCREEN_HEIGHT - 14);
		glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT - 14);
		glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		glVertex2f(0, SCREEN_HEIGHT);
		glEnd();
		c.render();

		//glScalef(0.001, 0.001, 0.001);
		glRotatef(g_rotate, 0, 0, 1.0);
		glTranslatef(0, 0, 1.0);

		glRasterPos2f(2, SCREEN_HEIGHT - 2);

		// TODO Object::drawHud
		stringstream buf;
		if (cube)
		{
			buf << "Moves: " << cube->totalMoves();
			buf << ", " << (cube->isValid() ? "valid" : "invalid");
			if (cube->isMade()) buf << ", perfect";
		}
		if (cmdQueue.size())
			buf << ", pgm size: " << cmdQueue.size();
		else if (cube && cube->isReady())
			buf << ", ready !";

		drawText(buf.str().c_str());
		glTranslatef(0, 0, -1.0);
	}

	long redisplay = Widget::renderAll();
	if (redisplay)
	{
		if (lastUpdateWidget == 0 || (redisplay < lastUpdateWidget))
		{
			lastUpdateWidget = redisplay;
			glutTimerFunc(redisplay, widgetUpdate, 0);
		}
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawScene()
{
	/*if (!server->running())
	  exit(1);
	 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	background.render();
	glTranslatef(cubex, cubey, cubez);
	
	glMultMatrixf(glm::value_ptr(orient));
	glScalef(scale, scale, scale);
	glRotatef(_anglex, 1.0, 0.0, 0.0);
	glRotatef(_angley, 0.0, 1.0, 0.0);
	//glUniformMatrix4fv(cube_server, 1, GL_FALSE, glm::value_ptr(orient));
	ObjectBuilder::render(false);

	if (bAxis)
	{
		glPushMatrix();

		// apply rotations
		// move the axes to the screen corner
		// draw our axes
		glBegin(GL_LINES);
		// draw line for x axis
		glColor3f(1.0, 0.0, 0.0); // X ROUGE
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(5.0, 0.0, 0.0);
		// draw line for y axis
		glColor3f(0.0, 1.0, 0.0); // Y VERT
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 5.0, 0.0);
		// draw line for Z axis
		glColor3f(0.0, 0.0, 1.0); // Z BLEU
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 5.0);
		glEnd();

		glLoadIdentity();
		glVertex3f(15, 0, 0);
		glEnd();
		glPopMatrix();
	}

	if (cube) cube->render(resetTimer);

	if (backward)
	{
		glLoadIdentity();
		glRotatef(180, 0.0, 1.0, 0.0);
		//glMultMatrixf(glm::value_ptr(orient));
		glTranslatef(back_x, back_y, back_z);
		glScalef(back_s, back_s, back_s);
		glRotatef(-10, 1.0, 1.0, 0.0);
		if (cube) cube->render(resetTimer);
	}
	if (flat)
	{
		glLoadIdentity();
		glTranslatef(flatx, flaty, flatz);
		glScalef(flats, flats, flats);
		if (cube) cube->renderFlat();
	}

	resetTimer = false;
	drawHud(true);
	glutSwapBuffers();
}

void update(int value)
{
	string msg = Widget::popMessage();
	if (msg.length())
	{
		if (msg.find(":") != string::npos && msg.find("EVENT mousedown") != string::npos)
		{
			string cmd(msg);
			StringUtil::getWord(cmd, ':');
			if (cmd.substr(0, 5) == "stack")
				cmdQueue.push_front(cmd);
			else
			{
				cmdQueue.push_back(cmd);
				cout << "Pushing [" << cmd << "]" << endl;
			}
		}

		server->send(msg);
	}


	int redisplay = 0;
	if (animy != 0 || animx != 0)
	{
		_angley += animy;
		if (_angley > 360) _angley -= 360;
		if (_angley < 0) _angley += 360;
		_anglex += animx;
		if (_anglex > 360)_anglex -= 360;
		if (_anglex < 0) _anglex += 360;

		redisplay = 479;
	}

	if (server)
	{
		if (server->incoming())
		{
			static string last;
			string incoming = server->getIncoming();

			if (incoming[0] == '.')
				incoming = last;
			else
				last = incoming;
			cmdQueue.push_back(incoming);

		}

		if (cmdQueue.size() && (cube == 0 || cube->isReady()))
		{
			redisplay = 499;
			string row = cmdQueue.front();
			string org_row = row;

			cmdQueue.pop_front();

			while (row[0] == '@')
				row.erase(0, 1);
			string org = row;

			string incoming = getWord(row, ";");
			trim(row);
			if (row.length())
				cmdQueue.push_front(row);

			Widget::replaceVars(incoming);
			cout << "INCOMMING " << incoming << endl;
			string cmd = getWord(incoming);
			cout << "CMD=[" << cmd << "] incoming=[" << incoming << "]" << endl;

			//if (StringUtil::match("[a-zA-Z]+[a-zA-Z0-9]*.[a-zA-Z]+[a-zA-Z0-9]+=", cmd))
			
			if (cmd=="match")
			{
				string s1=StringUtil::getWord(incoming);
				string s2=StringUtil::getWord(incoming);
				cout << Ansi::cyan() << "Match " << s1 << " / " << s2 << endl;
				if (StringUtil::preg_match(s1,s2,false))
					cout << Ansi::green() << "OK" << endl;
				else
					cout << Ansi::red() << "KO" << endl;
				cout << Ansi::reset();
			}
			else if (StringUtil::preg_match("^[a-zA-Z]+[a-zA-Z0-9_]*\\.[a-zA-Z]+[a-zA-Z0-9_]*", cmd, false))
			{
				cout << Ansi::cyan() << "MATCH" << endl;
				string name=StringUtil::getWord(cmd, '.');
				Object* object=ObjectBuilder::getInstance(name);
				if (object)
				{
					if (object->execute(server, cmd, incoming, org))
						server->send("#OK "+name+'.'+cmd);
					else
						server->send("#KO "+name+'.'+cmd);
				}
				else
				{
					server->send("#KO Object "+name+" unknown.");
				}
				cout << Ansi::reset();
			}
			else if (cmd == "help")
			{
				Help help;
				server->send("");
				server->send("Commands :");
				help.add("anim x|y value");
				help.add("axis");
				help.add("event {event} {allow|filter}");
				help.add("exec file");
				help.add("flat [x y z [scale]]");
				help.add("front color");
				help.add("hud [color]");
				help.add("is_made");
				help.add("is_valid [why]");
				help.add("new {object}");
				help.add("macro");
				help.add("pattern");
				help.add("peek/pop/push [name]/stack remember stack of cubes");
				help.add("quit");
				help.add("reset, reboot");
				help.add("scale s");
				help.add("screen x y");
				help.add("widget");
				help.add("var var=value");

				ObjectBuilder::help(help);

				int count=0;
				// TODO modify widget with new help style
				Widget::help(incoming);
				for (auto it : help.get())
				{
					if (incoming.length()==0 || it.first.find(incoming)!=string::npos)
					{
						count++;
						server->send(it.first);
					}
				}
				if (count==0)
					server->send("No other help on '"+incoming+"'");

				server->send("");
			}
			else if (cmd == "algo")
			{
				static map<char, map<char, char>> face_rotate;

				if (face_rotate.size() == 0)
				{
					map<char, char> faces;

					faces['t'] = 'f';
					faces['f'] = 'd';
					faces['d'] = 'b';
					faces['b'] = 't';
					faces['e'] = 'S';
					faces['s'] = 'e';
					face_rotate['x'] = faces; // bug b'e'b2eeb't2beeb2ebt2

					faces = map<char, char>();
					faces['f'] = 'r';
					faces['r'] = 'b';
					faces['b'] = 'l';
					faces['l'] = 'f';
					face_rotate['y'] = faces;

					faces = map<char, char>();
					faces['t'] = 'r';
					faces['r'] = 'd';
					faces['d'] = 'l';
					faces['l'] = 't';
					face_rotate['z'] = faces;

				}
				cmd = getWord(incoming);
				if (cmd == "rotate")
				{
					string axes = getWord(incoming);
					string algo;
					string new_algo = incoming;
					while (algo.find(' ') != string::npos)
						algo.erase(algo.find(' '), 1);

					while (axes.length())
					{
						auto it = face_rotate.find(axes[0]);
						axes.erase(0, 1);
						if (it != face_rotate.end())
						{
							map<char, char> rotate = it->second;

							algo = new_algo;
							new_algo = "";
							while (algo.length())
							{
								if (algo.length() == 0) break;

								bool reverse = false;
								char face = algo[0];
								algo.erase(0, 1);

								auto itr = rotate.find(face);

								if (itr != rotate.end())
								{
									char newface = itr->second;
									if (newface < 'Z')
									{
										newface |= 32;
										reverse = true;
									}
									new_algo += newface;
								}
								else
									new_algo += face;
								while (algo[0] == '\'' || algo[0] == '2')
								{
									if (algo[0] == '\'')
										reverse = !reverse;
									else
										new_algo += algo[0];
									algo.erase(0, 1);
								}
								if (reverse) new_algo += '\'';
							}
						}
					}
					cout << "rotate = " << new_algo << endl;
					server->send("#OK algo rotate=" + new_algo);
				}
				else
					server->send("#KO algo, unknown subcommand " + cmd);
			}
			else if (cmd == "new")
			{
				string obj_class = StringUtil::getWord(incoming);
				Object* object = ObjectBuilder::buildInstance(obj_class, incoming);

				if (object)
				{
					server->send("#OK new " + obj_class + " : " + object->getName());
				}
				else
					server->send("#KO new " + obj_class);
			}
			else if (cmd == "delete")
			{
				while (incoming.length())
				{
					string name(StringUtil::getWord(incoming));
					if (ObjectBuilder::destroyInstance(name))
						server->send("#OK " + name + " deleted.");
					else
						server->send("#KO no object with name " + name);
				}
			}
			else if (cmd == "objects")
			{
				string list = "object list: ";
				
				server->send("#OK object list : " + ObjectBuilder::listAll());
			}
			else if (cmd == "var")
			{
				Widget::setVar(incoming);
			}
			else if (cmd == "widget")
			{
				try
				{
					Widget* widget = Widget::factory(incoming);
					if (widget)
						server->send("#OK widget");
					else
						server->send("#KO widget");
				}
				catch (const char* p)
				{
					cerr << "CATCH " << p << endl;
					server->send(string("#KO widget ") + p);
				}
				catch (...)
				{
					cerr << "CATCH DURING WIDGET::FACTORY" << endl;
					server->send("#KO widget exception");
				}
			}
			else if (cmd == "scale")
			{
				scale = getFloat(incoming);
				server->send("#OK scale " + toString(scale));
			}
			else if (cmd == "hud")
			{
				if (incoming.length())
				{
					if (incoming == "reset")
					{
						hudr = hudr_org;
						hudv = hudv_org;
						hudb = hudb_org;
					}
					else
					{
						hudr = getFloat(incoming);
						if (incoming.length()) hudv = getFloat(incoming);
						if (incoming.length()) hudb = getFloat(incoming);
					}
					hud = true;
				}
				else
				{
					hud = !hud;
				}
				stringstream buf;
				buf << "#OK hud (" << (hud ? "ON" : "OFF") << ") (" << hudr << ", " << hudv << ", " << hudb << ")";
				server->send(buf.str());
			}
			else if (cmd == "flat")
			{
				if (incoming.length())
				{
					if (incoming == "reset")
					{
						flatx = flatx_org;
						flaty = flaty_org;
						flatz = flatz_org;
						flats = flats_org;
					}
					else
					{
						flatx = getFloat(incoming);
						if (incoming.length()) flaty = getFloat(incoming);
						if (incoming.length()) flatz = getFloat(incoming);
						if (incoming.length()) flats = getFloat(incoming);
					}
					flat = true;
				}
				else
				{
					flat = !flat;
				}
				stringstream buf;
				buf << "#OK flat (" << (flat ? "ON" : "OFF") << ") (" << flatx << ", " << flaty << ", " << flatz << ") scale=" << flats;
				server->send(buf.str());
			}
			else if (cmd == "translate")
			{
				if (incoming.length())
				{
					if (incoming == "reset")
					{
						cubex = cubex_org;
						cubey = cubey_org;
						cubez = cubez_org;
						scale = scale_org;
					}
					else
					{
						cubex = getFloat(incoming);
						if (incoming.length()) cubey = getFloat(incoming);
						if (incoming.length()) cubez = getFloat(incoming);
						if (incoming.length()) scale = getFloat(incoming);
					}
				}
				stringstream buf;
				buf << "#OK translate (" << cubex << ", " << cubey << ", " << cubez << ") scale=" << scale;
				server->send(buf.str());
			}
			else if (cmd == "axis")
			{
				bAxis = !bAxis;
				server->send("#OK axis RGB=XYZ");
			}
			else if (cmd == "color")
			{
				string dir;
				Face::Dir d = getDir(incoming, dir);

				if (d != Face::NONE)
				{
					server->send("#OK " + cube->face(d)->get(4).name());
				}
				else
					server->send("#KO invalid direction " + dir);
			}
			else if (cmd == "is_valid")
			{
				if (cube->isValid())
					server->send("#OK is_valid");
				else
				{
					server->send("#KO is_valid");
					if (incoming == "why")
					{
						cmdQueue.push_front("@find w b");
						cmdQueue.push_front("@find w o");
						cmdQueue.push_front("@find w g");
						cmdQueue.push_front("@find w r");
						cmdQueue.push_front("@find y b");
						cmdQueue.push_front("@find y o");
						cmdQueue.push_front("@find y g");
						cmdQueue.push_front("@find y r");
						cmdQueue.push_front("@find w b o");
						cmdQueue.push_front("@find w b r");
						cmdQueue.push_front("@find w g o");
						cmdQueue.push_front("@find w g r");
						cmdQueue.push_front("@find y b r");
						cmdQueue.push_front("@find y b o");
						cmdQueue.push_front("@find y g o");
						cmdQueue.push_front("@find y g r");
					}
				}
			}
			else if (cmd == "desc")
			{
				string dir;
				Face::Dir d = getDir(incoming, dir);
				if (d != Face::NONE)
				{
					string desc = cube->desc(d);
					server->send("#OK desc=" + desc);
				}
				else
					server->send("#KO desc");
			}
			else if (cmd == "is_made")
			{
				if (cube->isMade())
					server->send("#OK is_made CUBE IS PERFECT !");
				else
					server->send("#KO is_made");
			}
			else if (cmd == "front")
			{
				bool ok = true;
				const Color* front = getColor(incoming);
				if (front)
				{
					string face = cube->find(front);
					if (face == "front")
					{
					}
					else if (face == "bottom")
						cmdQueue.push_front("@rotate x -90");
					else if (face == "top")
						cmdQueue.push_front("@rotate x 90");
					else if (face == "right")
						cmdQueue.push_front("@rotate y -90");
					else if (face == "back")
					{
						cmdQueue.push_front("@rotate y 90");
						cmdQueue.push_front("@rotate y 90");
					}
					else if (face == "left")
						cmdQueue.push_front("@rotate y 90");
					else
						ok = false;
				}
				if (ok)
					server->send("#OK front");
				else
					server->send("#KO front");
			}
			else if (cmd == "top")
			{
				bool ok = true;
				const Color* top = getColor(incoming);
				if (top)
				{
					string face = cube->find(top);
					if (face == "top")
					{
					}
					else if (face == "bottom")
					{
						cmdQueue.push_front("@rotate x 90");
						cmdQueue.push_front("@rotate x 90");
					}
					else if (face == "front")
						cmdQueue.push_front("@rotate x -90");
					else if (face == "right")
						cmdQueue.push_front("@rotate z -90");
					else if (face == "back")
						cmdQueue.push_front("@rotate x 90");
					else if (face == "left")
						cmdQueue.push_front("@rotate z 90");
					else
						ok = false;
				}
				if (ok)
					server->send("#OK top");
				else
					server->send("#KO top");
			}
			else if (cmd == "orient")
			{
				cout << "orient " << incoming << ' ';
				string in2(incoming);
				const Color* c1 = getColor(in2);
				const Color* c2 = getColor(in2);
				string sc1 = getWord(incoming);
				string sc2 = getWord(incoming);

				cout << c1 << ' ' << c2 << endl;

				if (c1 && c2)
				{
					cmdQueue.push_front("@orient " + sc2); // For later

					string face = cube->find(c1); // c1 to top
					cout << "  face to top " << sc1 << "=" << face << endl;
					if (face == "front")
						cmdQueue.push_front("@rotate x -90");
					else if (face == "back")
						cmdQueue.push_front("@rotate x 90");
					else if (face == "left")
						cmdQueue.push_front("@rotate z 90");
					else if (face == "right")
						cmdQueue.push_front("@rotate z -90");
					else if (face == "bottom")
					{
						cmdQueue.push_front("@rotate x 90"); // 180 ne fonctionne pas
						cmdQueue.push_front("@rotate x 90");
					}

				}
				else if (c1)
				{
					bool bOk = true;
					string face = cube->find(c1); // c1 to front

					cout << "  face to front " << sc1 << "=" << face << endl;
					if (face == "back")
					{
						cmdQueue.push_front("@rotate y 90"); // 180 TODO
						cmdQueue.push_front("@rotate y 90");
					}
					else if (face == "left")
						cmdQueue.push_front("@rotate y 90");
					else if (face == "right")
						cmdQueue.push_front("@rotate y -90");
					else
						bOk = false;

					if (bOk)
						server->send("#OK orient");
					else
						server->send("#KO orient 634");

				}
				else
				{
					server->send("#KO orient " + incoming);
				}
			}
			else if (cmd == "event")
			{
				string op = StringUtil::getWord(incoming);
				if (incoming.length() && (op == "allow" || op == "filter"))
					Widget::filtersOp(incoming, op == "filter");
				else if (op == "sim")
				{
					string evt = StringUtil::getWord(incoming);
					Widget::pushEvent(evt, incoming);
				}
				else
					server->send("#KO event, unknown operation : " + op);
			}
			else if (cmd == "pattern" || cmd == "macro")
			{
				string what = getWord(incoming);
				string args = "";

				map<string, string>* container = &macros;
				if (cmd == "pattern")
					container = &patterns;
				else if (what == "exec")
				{
					what = getWord(incoming);
					args = ' ' + incoming;
					incoming = "";
				}

				if (what == "help")
				{
					server->send("  " + cmd + " [noreset] name        run the " + cmd + " name");
					server->send("  " + cmd + " name new_def          define the " + cmd + " name");
					server->send("  " + cmd + " delete name           delete the " + cmd + " name");
					server->send("  " + cmd + " load [file]           load a " + cmd + " file (default is " + cmd + "s.cub)");
					server->send("  " + cmd + " save [file]           save a " + cmd + " file");
				}
				else if (what == "save")
				{
					ofstream f(cmd + "s.cub");
					for (auto it = container->begin(); it != container->end(); it++)
						f << cmd << " " << it->first << " " << it->second << endl;
				}
				else if (what == "load")
				{
					run(cmd + "s.cub");
				}
				else if (what == "list")
				{
					string list;
					for (auto it = container->begin(); it != container->end(); it++)
						list += it->first + ' ';
					server->send("#OK " + cmd + " list=" + list);
				}
				else if (what == "delete")
				{
					auto it = container->find(incoming);
					if (it != container->end())
					{
						container->erase(it);
						cmdQueue.push_front('@' + cmd + " save");
						server->send("#OK " + cmd + " delete: '" + incoming + "'");
						if (cmd == "macro") cmdQueue.push_front("@macro save");
					}
					else
						server->send("#KO " + cmd + " delete: '" + incoming + "' not found");
				}
				else if (incoming.size() && what != "noreset")
				{
					string moves;
					if (incoming == "learn")
					{
						if (cube) moves = cube->learned();
					}
					else
						moves = incoming;
					(*container)[what] = moves;
					cmdQueue.push_front('@' + cmd + " save");
					server->send("#OK " + cmd + " " + what + " is defined");
				}
				else
				{
					auto it = container->find(what);
					if (it != container->end())
					{
						cmdQueue.push_front("@send #OK " + cmd);
						if (cmd == "pattern")
						{
							cmdQueue.push_front("@move " + it->second);
							if (incoming != "noreset") cmdQueue.push_front("@reset");
						}
						else
							cmdQueue.push_front('@' + it->second + args);
					}
					else
						server->send("#KO what (try " + cmd + " help)");
				}
			}
			else if (cmd == "orient")
			{
				const Color* c1 = getColor(incoming);
				const Color* c2 = getColor(incoming);
				if (c1 && c2)
				{

				}
				else
					server->send("#KO orient");
			}
			else if (cmd == "echo")
			{
				cout << "# " << incoming << endl;
				server->send("# " + incoming);
			}
			else if (cmd == "exec")
			{
				exec(incoming);
			}
			else if (cmd == "send")
			{
				cout << "SEND " << incoming << endl;
				server->send(incoming);
			}
			else if (cmd == "anim")
			{
				string a = getWord(incoming);
				if (a == "x")
					animx = getFloat(incoming);
				if (a == "y")
					animy = getFloat(incoming);
			}
			else if (cmd == "backward" || cmd == "back")
			{
				if (incoming.length())
				{
					backward = true;
					if (incoming == "reset")
					{
						back_x = 4.1;
						back_y = 4.1;
						back_z = 2.1;
						back_s = 0.4;
					}
					else
					{
						back_x = getFloat(incoming);
						if (incoming.length()) back_y = getFloat(incoming);
						if (incoming.length()) back_z = getFloat(incoming);
						if (incoming.length()) back_s = getFloat(incoming);
					}
				}
				else
					backward = !backward;
				stringstream buf;
				buf << setprecision(2) << "#OK backward=(" << back_x << ',' << back_y << ',' << back_z << ") scale=" << back_s;
				server->send(buf.str());
			}
			else if (cmd == "quit")
			{
				cout << "QUIT !" << endl;
				server->stop();
				usleep(1000000);

				onclose();
			}
			else if (cmd == "reboot")
			{
				reboot();
			}
			else if (cmd == "reset")
			{
				orient = glm::mat4();
				if (cube) cube->reset();
				server->send("#OK");
				_anglex = 0;
				_angley = 0;
			}
			else if (cmd == "run")
			{
				run(incoming);
			}
			else if (cmd == "screen")
			{
				sizex = getFloat(incoming);
				sizey = getFloat(incoming);
				if (sizex < 100) sizex = 100;
				if (sizey < 100) sizey = 100;
				if (sizex > 4000) sizex = 4000;
				if (sizey > 4000) sizey = 4000;
				glutReshapeWindow(sizex, sizey);
			}
			else if (cmd == "rotate")
			{
				trim(incoming);

				string invalid;
				while (incoming.length())
				{
					string dir;
					Face::Dir face = getDir(incoming, dir);
					float angle = getFloat(incoming);

					if (face != Face::NONE)
					{
						cube->rotate(face, angle);
					}
					else
					{
						bool clockwise = false;
						if (angle < 0) clockwise = true;
						if (dir == "x")
							cube->rotatex(clockwise);
						else if (dir == "y")
							cube->rotatey(clockwise);
						else if (dir == "z")
							cube->rotatez(clockwise);
						else
							invalid += dir;
					}
				}
				if (invalid.length())
					server->send("#ERROR: Invalid direction [" + invalid + "]");
			}
			else if (cmd == "centers")
			{
				cmdQueue.push_back("find white");
				cmdQueue.push_back("find yellow");
				cmdQueue.push_back("find blue");
				cmdQueue.push_back("find orange");
				cmdQueue.push_back("find green");
				cmdQueue.push_back("find red");
			}
			else if (cmd == "find")
			{
				const Color* c1 = getColor(incoming);
				const Color* c2 = 0;
				const Color* c3 = 0;
				if (incoming.length()) c2 = getColor(incoming);
				if (incoming.length()) c3 = getColor(incoming);

				string where;

				cout << "find " << c1 << " " << c2 << " " << c3 << endl;

				if (c1 == 0)
					server->send("#KO");
				else if (c2 == 0)
				{
					where = cube->find(c1);
				}
				else if (c3 == 0)
				{
					where = cube->find(c1, c2);
				}
				else
				{
					where = cube->find(c1, c2, c3);
				}
				if (where.length())
				{
					cout << "found (" << where << ")" << endl;
					server->send("#OK " + org + "=" + where);
				}
				else
				{
					cout << "not found !" << endl;
					server->send("#KO " + org);
				}
				cout << "---------------" << endl;
			}
			else if (ObjectBuilder::execute(server, cmd, incoming, org))
			{
				cerr << "Ok ObjectBuilder.execute " << cmd << endl;
			}
			else if (Widget::onCommand(org_row))
				goto horrible; // @FIXME PTDR, un GOTO dans un code c++ Mouarf
			else if (macros.find(cmd) != macros.end())
			{
				cmdQueue.push_front("@macro exec " + cmd + " " + incoming);
			}
			else if (patterns.find(cmd) != patterns.end())
			{
				cmdQueue.push_front("@pattern " + cmd);
			}
			else
				server->send("#KO Unknown command [" + cmd + "]");
		}
	}

horrible:

	if (cube && !cube->isReady()) redisplay = 969;
	if (redisplay || redisplayAsked)
	{
		redisplayAsked = false;
		glutPostRedisplay();
	}
	else
		resetTimer = true;

	glutTimerFunc(10, update, 0);
}

void mouse(int button, int state, int x, int y)
{
	cout << "MOUSE " << button << '/' << state << endl;
	GLdouble pix[20];
	/* si on appuie sur le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		presse = 1; /* le booleen presse passe a 1 (vrai) */
		xold = x; /* on sauvegarde la position de la souris */
		yold = y;
	}
	else
	{
		glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, pix);

		cout << "depth=" << pix[0] << "  ";
	}
	/* si on relache le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse = 0; /* le booleen presse passe a 0 (faux) */

	if (Widget::mouseButton(button, state, x, y) == 0 && (button == 4 || button == 3))
	{
		if (button == 4)
			scale *= 0.98;
		else
			scale /= 0.98;
		cout << "SCALE MODIFY:" << scale << endl;
	}
}

void mousemotion(int x, int y)
{
	Widget::mouseMotion(x, y);
	if (presse) /* si le bouton gauche est presse */
	{
		/* on modifie les angles de rotation de l'objet
		   en fonction de la position actuelle de la souris et de la derniere
		   position sauvegardee */

		float dx = x - xold;
		float dy = y - yold;
		glm::vec3 axis_x(1, 0, 0);
		glm::vec3 axis_y(0, 1, 0);
		glm::vec3 axis_z(0, 0, 1);
		glm::vec4 delta(dx, dy, 0, 1);

		//delta = orient * delta;
		
		glm::mat4 delta_rot;
		
		delta_rot = glm::rotate(delta_rot, glm::radians(delta.x), axis_y);
		delta_rot = glm::rotate(delta_rot, glm::radians(delta.y), axis_x);
		
		// orient *= delta_rot;
		//orient = glm::rotate(orient, glm::radians(dy), axis_x);
		
		_anglex += dy;
		_angley +=dx;
		
		glm::vec4 rot(dy,dx, 0,1 );
		
		glm::mat4 trans = glm::transpose(orient);
		
		rot = trans * rot;
		
		_anglex += rot.x;
		_angley += rot.y;
		
		orient *= rot;
		
		
		/*	glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(glm::value_ptr(orient));
		 */
		/*
		   cout << setprecision(2);
		   cout << "Rotate x : " << dx << " : " << glm::radians(dx) << endl;
		   cout << orient[0][0] << endl;
		   float* ptr=glm::value_ptr(delta_rot);
		   for(int row=0; row<4; row++)
		   {
		   cout << "[ ";
		   for(int col=0; col<4; col++)
		   {
		   cout << *ptr++ << ' ';
		   }
		   cout << " ]" << endl;
		   }
		 */
		/*
		   angley = anglex+((xold-x)/3.0);
		   anglez = angley+((yold-y)/3.0);
		 */
		// cout << "anglex=" << anglex << " angley=" << angley << endl;
		glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
	}

	xold = x; /* sauvegarde des valeurs courante de le position de la souris */
	yold = y;
}

void mousepassivemotion(int x, int y)
{
	mousemotion(x, y);
}

void fillPatterns()
{
	if (patterns.size() == 0)
	{
		patterns["cube"] = "flfu'ruf2l2u'l'bd'b'l2u";
		patterns["cube3"] = " F D F' D2 L' B' U L D R U L' F' U L U2";
		patterns["checker"] = "eemmss";
		patterns["zigzag"] = "R L B F R L B F R L B F";
		patterns["holes"] = "U D' B F' R L' U D' ";
		patterns["6t"] = "f2r2u2f'bd2l2fb";
		patterns["stripes"] = "F U F R L2 B D' R D2 L D' B R2 L F U F";
		patterns["twisted"] = "F B' U F U F U L B L2 B' U F' L U L' B";
		patterns["flowers"] = "eemmssU D' B F' R L' U D'";
	}
}

int main(int argc, char** argv)
{
	//cmdQueue.push_back("rotate left 9000");
	//cmdQueue.push_back("rotate x");

	Widget::setCmdQueue(&cmdQueue);
	reboot();

	long lPort;
	if (argc == 2)
		lPort = atol(argv[1]);
	else
		lPort = 3333;

	Widget::setVar("port=" + std::to_string(lPort));
	server = new Server(lPort);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE_SGIS);
	glutInitWindowSize(sizex, sizey);

	string sPort = toString(lPort);
	glutCreateWindow((string("CS ") + sPort).c_str());
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutPassiveMotionFunc(mousepassivemotion);
	glutTimerFunc(25, update, 0);
	atexit(onclose);

	glutMainLoop();

	return 0;

}

