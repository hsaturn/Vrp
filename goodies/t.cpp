// A Simple GLUT application.
// If you like, use this as a skeleton for your assignments and project work
// Shows a scene which includes a sphere and a cylinder inside a conatiner box.
// Function Keys support:
//   1. F1 : toggle between ortho/perspective mode of viewing.
//   2. F2 : toggle between continuous rotation about vertical axis and stop rotation
//   3. Up and Down Arrow keys for rotating view about  horizontal axis
//	 4. F3 and F4 for zoom in and zoom out
//	 5.	F10: reset view in case your view parameters get out of control.


// Link with: opengl32.lib, glu32.lib, glut32.lib.
// glut includes links to the other two libraries.

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>

using namespace std;

// Initial size of graphics window on your screen.
const int WIDTH  = 600; // in pixels
const int HEIGHT = 600; //

// increment for idle function rotation and zoom
const double RotSTEP = 0.1;
const double ALL_ROUND = 360;
const double ZoomSTEP = 0.2;
const double zoomFactor = 1.03;

// Current size of window (will change when you resize window)
int width  = WIDTH;
int height = HEIGHT;

// Mouse positions, normalized to [0,1].
double xMouse = 0.5;
double yMouse = 0.5;

// Bounds of viewing frustum, maintain uniform scaling.
double viewWindowLeft =  -40;
double viewWindowRight  = 40;
double viewWindowBottom =  -40;
double viewWindowTop  = 40;
double nearPlane =  40;
double farPlane  = 120;

// Viewing angle.
double fovy = 60;

// Variables.
double alpha = 0;                                  // rotation angle about Y axis.
double beta = 0;                                   // rotation angle about X axis.
double halfway = - (farPlane + nearPlane) / 2;	   // half way between near and far planes

int idleEnable = 0;	// flags that set continuous rotation on/off
int projType = 0;   // flag for proj type; ortho = 0, perspective = 1

void resetViewParameters()
{
	// resets 3D synthetic camera parameters to default values

	viewWindowLeft =  -40;
	viewWindowRight  = 40;
	viewWindowBottom =  -40;
	viewWindowTop  = 40;
	nearPlane =  40;
	farPlane  = 120;
	fovy = 60;
	alpha = 0.0;
	beta = 0.0;
	projType = 0;
	idleEnable = 0;

}


void setView()
{
	// Must set it up in Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Rotation about X from UP-DOWN Arrow key
	glRotatef(beta, 1, 0, 0);


	// Rotation about Y from idle function.
	glRotatef(alpha, 0, 1, 0);

	if(projType)
		gluPerspective(fovy, (GLfloat) 1.0, nearPlane, farPlane);
	else
		glOrtho
		(
			viewWindowLeft,
			viewWindowRight,
			viewWindowBottom,
			viewWindowTop,
			nearPlane,
			farPlane
		);
}

void zoom(unsigned char direction)
{
   // for zoom in reduces viewWindow size and for zoom out increases it.

	if (direction == '+') {
			viewWindowLeft += ZoomSTEP; viewWindowRight -= ZoomSTEP;
			viewWindowBottom += ZoomSTEP; viewWindowTop -= ZoomSTEP;
			fovy = fovy / zoomFactor;
		}
		else if (direction == '-') {
			viewWindowLeft -= ZoomSTEP; viewWindowRight += ZoomSTEP;
			viewWindowBottom -= ZoomSTEP; viewWindowTop += ZoomSTEP;
			fovy = fovy * zoomFactor;
		}

}

void drawCylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLdouble x, GLdouble y, GLdouble z)
{

    // glut provides functions for cube and sphere, but not for cylinder, cone.
	// Hence this glu function has to be called.

	GLUquadricObj* cyl = NULL;
	cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_LINE);

	glPushMatrix();

		glTranslated(x, y, z);
		glTranslated(0, height, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(cyl, baseRadius, topRadius, height, 6, 4);
		gluDeleteQuadric(cyl);

	glPopMatrix();
}

void drawSphere(GLdouble Radius, GLdouble x, GLdouble y, GLdouble z)
{
	glPushMatrix();
		//	translate the table to its position
		glTranslated(x, y, z);
		//		draw glut sphere
		glutWireSphere(Radius, 20, 20);
	glPopMatrix();

}

void drawScene(GLdouble scenewidth, GLdouble sceneheight, GLdouble scenedepth)
{
	GLdouble cylRadius = 5.0;
	GLdouble sphRadius = 2.0;
	/* container box */
	glPushMatrix();
		glScaled(scenewidth, sceneheight, scenedepth);
		glutWireCube(1.0);
	glPopMatrix();

	/* put Cylinder in centre of box at the bottom... */
	drawCylinder(0.2, 0.2, cylRadius, 0.0, -sceneheight/2.0, 0.0);
	/* put sphere on top of cylinder */
	drawSphere(2.0, 0.0, -(cylRadius+sphRadius)/2.0, 0.0);

}


// This function is called to display the scene.
void display ()
{
	setView();

	glClear(GL_COLOR_BUFFER_BIT);

	// set modelling mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	position so that container box is placed within the near and far planes.
//	This will prevent them from getting clipped completely.
	glTranslated(0, 0, halfway);

   // Draw model axes in centre of room.
   glBegin(GL_LINES);
      // X axis
      glColor3f(1, 0, 0);
      glVertex3f(0, 0, 0);
      glVertex3f(10, 0, 0);
      // Y axis
      glColor3f(0, 1, 0);
      glVertex3f(0, 0, 0);
      glVertex3f(0, 10, 0);
      // Z axis
      glColor3f(0, 0, 1);
      glVertex3f(0, 0, 0);
      glVertex3f(0, 0, 10);
   glEnd();

   // Draw the scene in double buffer.
   glColor3f(0, 1, 1);
   drawScene(20.0, 20.0, 20.0);
   // now swap buffer
   glutSwapBuffers();
}

// This function is called when there is nothing else to do.
void idle ()
{

   if (idleEnable)
{	alpha += RotSTEP;
   if (alpha > ALL_ROUND)
      alpha -= ALL_ROUND;

   // Display normalized coordinates in title bar.
   const int BUFSIZE = 200;
   static char buffer[BUFSIZE];
   cout <<
      resetiosflags(ios::floatfield) << setiosflags(ios::fixed) <<
      setprecision(3) << "Mouse at (" << xMouse << ", " << yMouse <<
      setprecision(0) << ").  Alpha=" << setw(3) << alpha <<
      ".  Beta=" << setw(3) << beta <<
      setprecision(2) << ".  Distance=" << halfway <<
      "." << endl;
   glutSetWindowTitle(buffer);

	glutPostRedisplay();
   }
}

void mouseMovement (int mx, int my)
{
   // Normalize mouse coordinates.
   xMouse = double(mx) / double(width);
   yMouse = 1 - double(my) / double(height);

   // Redisplay image.
   glutPostRedisplay();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);

}

// Display help.
void help()
{
   cout << " F1 : toggle between ortho/perspective mode of viewing." << endl
        << " F2 : toggle between continuous rotation about vertical axis and stop rotation." << endl
        << " Up and Down Arrow keys for rotating view about  horizontal axis." << endl
        << " F3 and F4 for zoom in and zoom out." << endl
        << " F10: reset view in case your view parameters get out of control." << endl <<
      endl;
}

// Respond to graphic character keys.
// Parameters give key code and mouse coordinates.
void graphicKeys (unsigned char key, int x, int y)
{
	switch (key)
	{
   case 'h':
      help();
      break;
	case 27:
		exit(0);
   default:
      cout << key << endl;
	}
}

// Respond to function keys.
// Parameters give key code and mouse coordinates.
void functionKeys (int key, int x, int y)
{
   switch (key)
   {
   case GLUT_KEY_F10:
     resetViewParameters();
      break;
   case GLUT_KEY_F1:
     projType = 1 - projType;
      break;
   case GLUT_KEY_F2:
      idleEnable = 1 - idleEnable;
      break;
   case GLUT_KEY_F3:
      zoom('+');
      break;
   case GLUT_KEY_F4:
      zoom('-');
      break;
   case GLUT_KEY_UP:
      beta += 10*RotSTEP;
	  if (beta > ALL_ROUND)
			beta -= ALL_ROUND;
      break;
   case GLUT_KEY_DOWN:
      beta -= 10*RotSTEP;
	  if (beta < -ALL_ROUND)
			beta += ALL_ROUND;
      break;
   }

   	glutPostRedisplay();
}


int main (int argc, char **argv)
{
   // GLUT initialization. Enable double buffer mode
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GLUT Skeleton Program");

	// Register call backs for standard tasks such as:
	// display, windowresize, keypress, functionkey press, mouse movement.
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeMainWindow);
	glutKeyboardFunc(graphicKeys);
    glutSpecialFunc(functionKeys);
    glutMotionFunc(mouseMovement);
	glutIdleFunc(idle);

   // Display help.
	help();

   // Enter GLUT loop.
	glutMainLoop();
	return 0;
}

