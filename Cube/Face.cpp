#include <Face.h>
#include <GL/glut.h>
#include <Color.h>
#include <Cube.h>


const float Face::size = 1.0;
float Face::fRotationSpeed=180;
bool Face::mark=false;

Face::Face()
: angle(0.0), angle_target(0)
{
}

void Face::reset(Dir d, Color c)
{
	mDir = d;
	fill(c);
	angle=0;
	angle_target=0;
}

void Face::fill(const Color c)
{
	for (int i = 0; i<9; i++)
		colors[i] = c;
}

void Face::renderFlat(float size, Cube* c)
{
	size /=3.2;
	const float coeff = 0.9;
	const float halfc = size*coeff*0.5;

	glPushMatrix();
	
	for (int i = 0; i<9; i++)
	{
		glPopMatrix();
		glPushMatrix();

		//glRotateAngle();
		glRotateFlat();

		float offx;
		float offz;

		float col = (i%3)-1;
		float row = (i/3)-1;

		offx = size*(col);
		offz = size*(row);
		//glRotateFace(i, c, 0);
		glBegin(GL_QUADS);
		colors[i].render();
		glVertex2f(offx-halfc, offz-halfc);
		glVertex2f(offx-halfc, offz+halfc);
		glVertex2f(offx+halfc, offz+halfc);
		glVertex2f(offx+halfc, offz-halfc);
		glEnd();
		
		if (mark && i<=1)
		{
			glPopMatrix();
			glPushMatrix();
			glTranslatef(0.0,0.0,0.01);
			glRotateFlat();
			glBegin(GL_QUADS);
			Color::black.render();
			glVertex2f(offx-halfc/4, offz-halfc/4);
			glVertex2f(offx-halfc/4, offz+halfc/4);
			glVertex2f(offx+halfc/4, offz+halfc/4);
			glVertex2f(offx+halfc/4, offz-halfc/4);
			glEnd();
		}
	
		glPopMatrix();
		glPushMatrix();


	}
	glPopMatrix();
}


void Face::render(Cube* c)
{
	int sens = epsilon();
	const float coeff = 0.9;
	const float halfc = size*coeff*0.5;
	const float half = size*0.50002;
	float halfu = half*0.98;
	float offy = (float) sens*0.01;

	glPushMatrix();
	
	float y=sens*size+offy;
	float s=size*1.41;
		glTranslate();
		glRotate();
	glBegin(GL_QUADS);
	Color::dark_gray.render();
	glVertex3f(-s, y, -s);
	glVertex3f(-s, y, s);
	glVertex3f(s, y, s);
	glVertex3f(s, y, -s);
	glEnd();
	
	for (int i = 0; i<9; i++)
	{
		glPopMatrix();
		glPushMatrix();

			glRotateFace(i, c);
			glTranslate();
			glRotateAngle();
			glRotate();

		float offx;
		float offz;

		float col = (i%3)-1;
		float row = (i/3)-1;

		offx = size*(col);
		offz = size*(row);

		glBegin(GL_QUADS);
		Color::black.render();
		glVertex3f(offx-half, offy, offz-half);
		glVertex3f(offx-half, offy, offz+half);
		glVertex3f(offx+half, offy, offz+half);
		glVertex3f(offx+half, offy, offz-half);
		glEnd();

		glBegin(GL_QUADS);
		colors[i].render();
		glVertex3f(offx-halfc, 0, offz-halfc);
		glVertex3f(offx-halfc, 0, offz+halfc);
		glVertex3f(offx+halfc, 0, offz+halfc);
		glVertex3f(offx+halfc, 0, offz-halfc);
		glEnd();
		
			glBegin(GL_QUADS);
			Color::dark_gray.render();
			glVertex3f(offx-halfu, sens, offz-halfu);
			glVertex3f(offx-halfu, sens, offz+halfu);
			glVertex3f(offx+halfu, sens, offz+halfu);
			glVertex3f(offx+halfu, sens, offz-halfu);
			glEnd();
		
		if (mark && i<=1)
		{
			glBegin(GL_QUADS);
			Color::black.render();
			glVertex3f(offx-halfc/4, -0.01*sens, offz-halfc/4);
			glVertex3f(offx-halfc/4, -0.01*sens, offz+halfc/4);
			glVertex3f(offx+halfc/4, -0.01*sens, offz+halfc/4);
			glVertex3f(offx+halfc/4, -0.01*sens, offz-halfc/4);
			glEnd();
			
		}
	
		glPopMatrix();
		glPushMatrix();


	}
	glPopMatrix();
}

void Face::glRotateAngle() const
{
	switch (mDir)
	{
		case TOP:
			glRotatef(angle, 0, -1, 0);
			break;
		case BOTTOM:
			glRotatef(angle, 0, 1, 0);
			break;
		case LEFT:
			glRotatef(angle, 1, 0, 0);
			break;
		case RIGHT:
			glRotatef(angle, -1, 0, 0);
			break;
		case FRONT:
			glRotatef(angle, 0, 0, -1);
			break;
		case BACK:
			glRotatef(angle, 0, 0, 1);
			break;

		default:
			break;
	}
}

bool Face::isUniform() const
{
	bool bUniform=true;
	for(int i=1; i<9; i++)
	{
		if (colors[0]!=colors[i])
		{
			bUniform=false;
			break;
		}
	}
	return bUniform;
}

string Face::find(const Color* c1, const Color* c2, const Cube* cube) const
{
	Color	adj[12];
	Dir		dirs[4];
	fillAdjacentArray(adj, dirs, cube);
	
	string where="";
	for(int i=1; i<8; i+=2)
		if (colors[i]==*c1)
		{
			int j=i;
			if (i==3) j=10;
			if (i==5) j=4;
			if (epsilon()==1)
			{
				if (i==3) j=1;
				if (i==3 && (mDir==BOTTOM || mDir==BACK || mDir==RIGHT)) j=4;
				if (i==5) j=10;
			}
			//cout << "found c1 on " << i << " " << dirToString(mDir) << " e=" << epsilon() << " j=" << j<< endl;
			if (adj[j]==*c2)
				return dirToString(mDir)+" "+dirToString(dirs[j/3]);
		}
		
	return where;
}

string Face::find(const Color* c1, const Color* c2, const Color* c3, const Cube* cube) const
{
	Color adj[12];
	Dir		dirs[4];
	fillAdjacentArray(adj, dirs, cube);
	
	for(int i=0; i<9; )
	{
		if (colors[i]==*c1)
		{
			//cout << "found c1 on " << dirToString(mDir) << ':' << i << endl;
			int j2=i;
			int j3=11;
			if (epsilon()==1)
			{
				if (i==0) { j2=5; j3=0; }
				if (i==2) { j2=2; j3=9; }
				if (i==6) { j2=8; j3=3; }
				if (i==8) { j2=11; j3=6; }
			}
			else
			{
				if (i==2) { j2=2; j3=3; }
				if (i==6) { j2=8; j3=9; }
				if (i==8) { j2=5; j3=6; }
			}
			//cout << "adj=" << j2 << ' ' << j3 << endl;
			if (adj[j2]==*c2 && adj[j3]==*c3)
				return dirToString(mDir)+" "+dirToString(dirs[j2/3])+" "+dirToString(dirs[j3/3]);
			else if (adj[j3]==*c2 && adj[j2]==*c3)
				return dirToString(mDir)+" "+dirToString(dirs[j3/3])+" "+dirToString(dirs[j2/3]);
		}
		if (i==2)
			i=6;
		else
			i+=2;
	}
	return "";
}

void Face::fillAdjacentArray(Color* nc, Dir* dirs, const Cube* cube) const
{
	// 2 - Fill contour
	switch (mDir)
	{
		case TOP:
			nc[0] = cube->face(BACK)->colors[0];
			nc[1] = cube->face(BACK)->colors[1];
			nc[2] = cube->face(BACK)->colors[2];
			nc[3] = cube->face(RIGHT)->colors[2];
			nc[4] = cube->face(RIGHT)->colors[5];
			nc[5] = cube->face(RIGHT)->colors[8];
			nc[6] = cube->face(FRONT)->colors[2];
			nc[7] = cube->face(FRONT)->colors[1];
			nc[8] = cube->face(FRONT)->colors[0];
			nc[9] = cube->face(LEFT)->colors[8];
			nc[10] = cube->face(LEFT)->colors[5];
			nc[11] = cube->face(LEFT)->colors[2];
			if (dirs)
			{
				dirs[0]=BACK;
				dirs[1]=RIGHT;
				dirs[2]=FRONT;
				dirs[3]=LEFT;
			}
			break;

		case BOTTOM:
			nc[0] = cube->face(BACK)->colors[6];
			nc[1] = cube->face(BACK)->colors[7];
			nc[2] = cube->face(BACK)->colors[8];
			nc[3] = cube->face(LEFT)->colors[6];
			nc[4] = cube->face(LEFT)->colors[3];
			nc[5] = cube->face(LEFT)->colors[0];
			nc[6] = cube->face(FRONT)->colors[8];
			nc[7] = cube->face(FRONT)->colors[7];
			nc[8] = cube->face(FRONT)->colors[6];
			nc[9] = cube->face(RIGHT)->colors[0];
			nc[10] = cube->face(RIGHT)->colors[3];
			nc[11] = cube->face(RIGHT)->colors[6];
			if (dirs)
			{
				dirs[0]=BACK;
				dirs[1]=LEFT;
				dirs[2]=FRONT;
				dirs[3]=RIGHT;
			}
			break;

		case LEFT:
			nc[0] = cube->face(BACK)->colors[6];
			nc[1] = cube->face(BACK)->colors[3];
			nc[2] = cube->face(BACK)->colors[0];
			nc[3] = cube->face(TOP)->colors[0];
			nc[4] = cube->face(TOP)->colors[3];
			nc[5] = cube->face(TOP)->colors[6];
			nc[6] = cube->face(FRONT)->colors[0];
			nc[7] = cube->face(FRONT)->colors[3];
			nc[8] = cube->face(FRONT)->colors[6];
			nc[9] = cube->face(BOTTOM)->colors[6];
			nc[10] = cube->face(BOTTOM)->colors[3];
			nc[11] = cube->face(BOTTOM)->colors[0];
			if (dirs)
			{
				dirs[0]=BACK;
				dirs[1]=TOP;
				dirs[2]=FRONT;
				dirs[3]=BOTTOM;
			}			
			break;
			
		case RIGHT:
			nc[0] = cube->face(BACK)->colors[8];
			nc[1] = cube->face(BACK)->colors[5];
			nc[2] = cube->face(BACK)->colors[2];
			nc[3] = cube->face(BOTTOM)->colors[8];
			nc[4] = cube->face(BOTTOM)->colors[5];
			nc[5] = cube->face(BOTTOM)->colors[2];
			nc[6] = cube->face(FRONT)->colors[2];
			nc[7] = cube->face(FRONT)->colors[5];
			nc[8] = cube->face(FRONT)->colors[8];
			nc[9] = cube->face(TOP)->colors[2];
			nc[10] = cube->face(TOP)->colors[5];
			nc[11] = cube->face(TOP)->colors[8];
			if (dirs)
			{
				dirs[0]=BACK;
				dirs[1]=BOTTOM;
				dirs[2]=FRONT;
				dirs[3]=TOP;
			}
			break;
			
		case FRONT:
			nc[0] = cube->face(TOP)->colors[6];
			nc[1] = cube->face(TOP)->colors[7];
			nc[2] = cube->face(TOP)->colors[8];
			nc[3] = cube->face(RIGHT)->colors[8];
			nc[4] = cube->face(RIGHT)->colors[7];
			nc[5] = cube->face(RIGHT)->colors[6];
			nc[6] = cube->face(BOTTOM)->colors[8];
			nc[7] = cube->face(BOTTOM)->colors[7];
			nc[8] = cube->face(BOTTOM)->colors[6];
			nc[9] = cube->face(LEFT)->colors[6];
			nc[10] = cube->face(LEFT)->colors[7];
			nc[11] = cube->face(LEFT)->colors[8];
			if (dirs)
			{
				dirs[0]=TOP;
				dirs[1]=RIGHT;
				dirs[2]=BOTTOM;
				dirs[3]=LEFT;
			}
			break;
		case BACK:
			nc[0] = cube->face(TOP)->colors[0];
			nc[1] = cube->face(TOP)->colors[1];
			nc[2] = cube->face(TOP)->colors[2];
			nc[3] = cube->face(LEFT)->colors[0];
			nc[4] = cube->face(LEFT)->colors[1];
			nc[5] = cube->face(LEFT)->colors[2];
			nc[6] = cube->face(BOTTOM)->colors[2];
			nc[7] = cube->face(BOTTOM)->colors[1];
			nc[8] = cube->face(BOTTOM)->colors[0];
			nc[9] = cube->face(RIGHT)->colors[2];
			nc[10] = cube->face(RIGHT)->colors[1];
			nc[11] = cube->face(RIGHT)->colors[0];
			if (dirs)
			{
				dirs[0]=TOP;
				dirs[1]=LEFT;
				dirs[2]=BOTTOM;
				dirs[3]=RIGHT;
			}
			break;
		default:
			cerr << "WHAT 293 ?" << endl;
			break;
	}
}

void swap(Color* c, int from, int to)
{
	Color m=c[from];
	c[from]=c[to];
	c[to]=m;
}

void Face::swapFaces(bool left_right)
{
	if (!left_right)
	{
		swap(colors,0,6);
		swap(colors,1,7);
		swap(colors,2,8);
	}
	else
	{
		swap(colors,0,2);
		swap(colors,3,5);
		swap(colors,6,8);
	}
	//colors[0]=Color::dark_gray;
}

void Face::rotateFaces(bool clockWise, Cube* cube)
{
	Color nc[12];

	if (clockWise ^ (epsilon()==-1))	// TOP -1 ok
	{
		nc[0] = colors[6];
		nc[1] = colors[3];
		nc[2] = colors[0];
		nc[3] = colors[7];
		nc[4] = colors[4];
		nc[5] = colors[1];
		nc[6] = colors[8];
		nc[7] = colors[5];
		nc[8] = colors[2];
	}
	else
	{
		nc[0] = colors[2];
		nc[1] = colors[5];
		nc[2] = colors[8];
		nc[3] = colors[1];
		nc[4] = colors[4];
		nc[5] = colors[7];
		nc[6] = colors[0];
		nc[7] = colors[3];
		nc[8] = colors[6];
	}

	for (int i = 0; i<9; i++)
		colors[i] = nc[i];
	
	if (cube==0) return;

	fillAdjacentArray(nc, 0, cube);

	// 3 - rotate
	Color rnc[12];
	if (clockWise)
	{
		for (int i = 0; i<9; i++)
			rnc[i] = nc[i+3];
		for (int i = 9; i<12; i++)
			rnc[i] = nc[i-9];
	}
	else
	{
		for (int i = 3; i<12; i++)
			rnc[i] = nc[i-3];
		for (int i = 0; i<3; i++)
			rnc[i] = nc[i+9];
	}

	// 4 - reaffect colors
	switch (mDir)
	{
		case TOP:
			cube->face(BACK)->colors[0] = rnc[0];
			cube->face(BACK)->colors[1] = rnc[1];
			cube->face(BACK)->colors[2] = rnc[2];
			cube->face(RIGHT)->colors[2] = rnc[3];
			cube->face(RIGHT)->colors[5] = rnc[4];
			cube->face(RIGHT)->colors[8] = rnc[5];
			cube->face(FRONT)->colors[2] = rnc[6];
			cube->face(FRONT)->colors[1] = rnc[7];
			cube->face(FRONT)->colors[0] = rnc[8];
			cube->face(LEFT)->colors[8] = rnc[9];
			cube->face(LEFT)->colors[5] = rnc[10];
			cube->face(LEFT)->colors[2] = rnc[11];
			break;

		case BOTTOM:
			cube->face(BACK)->colors[6] = rnc[0];
			cube->face(BACK)->colors[7] = rnc[1];
			cube->face(BACK)->colors[8] = rnc[2];
			cube->face(LEFT)->colors[6] = rnc[3];
			cube->face(LEFT)->colors[3] = rnc[4];
			cube->face(LEFT)->colors[0] = rnc[5];
			cube->face(FRONT)->colors[8] = rnc[6];
			cube->face(FRONT)->colors[7] = rnc[7];
			cube->face(FRONT)->colors[6] = rnc[8];
			cube->face(RIGHT)->colors[0] = rnc[9];
			cube->face(RIGHT)->colors[3] = rnc[10];
			cube->face(RIGHT)->colors[6] = rnc[11];
			break;
			
		case LEFT:
			cube->face(BACK)->colors[6]=rnc[0];
			cube->face(BACK)->colors[3]=rnc[1];
			cube->face(BACK)->colors[0]=rnc[2];
			cube->face(TOP)->colors[0]=rnc[3];
			cube->face(TOP)->colors[3]=rnc[4];
			cube->face(TOP)->colors[6]=rnc[5];
			cube->face(FRONT)->colors[0]=rnc[6];
			cube->face(FRONT)->colors[3]=rnc[7];
			cube->face(FRONT)->colors[6]=rnc[8];
			cube->face(BOTTOM)->colors[6]=rnc[9];
			cube->face(BOTTOM)->colors[3]=rnc[10];
			cube->face(BOTTOM)->colors[0]=rnc[11];
			break;
		case RIGHT:
			cube->face(BACK)->colors[8]=rnc[0];
			cube->face(BACK)->colors[5]=rnc[1];
			cube->face(BACK)->colors[2]=rnc[2];
			cube->face(BOTTOM)->colors[8]=rnc[3];
			cube->face(BOTTOM)->colors[5]=rnc[4];
			cube->face(BOTTOM)->colors[2]=rnc[5];
			cube->face(FRONT)->colors[2]=rnc[6];
			cube->face(FRONT)->colors[5]=rnc[7];
			cube->face(FRONT)->colors[8]=rnc[8];
			cube->face(TOP)->colors[2]=rnc[9];
			cube->face(TOP)->colors[5]=rnc[10];
			cube->face(TOP)->colors[8]=rnc[11];
			break;
			
		case FRONT:
			cube->face(TOP)->colors[6]=rnc[0];
			cube->face(TOP)->colors[7]=rnc[1];
			cube->face(TOP)->colors[8]=rnc[2];
			cube->face(RIGHT)->colors[8]=rnc[3];
			cube->face(RIGHT)->colors[7]=rnc[4];
			cube->face(RIGHT)->colors[6]=rnc[5];
			cube->face(BOTTOM)->colors[8]=rnc[6];
			cube->face(BOTTOM)->colors[7]=rnc[7];
			cube->face(BOTTOM)->colors[6]=rnc[8];
			cube->face(LEFT)->colors[6]=rnc[9];
			cube->face(LEFT)->colors[7]=rnc[10];
			cube->face(LEFT)->colors[8]=rnc[11];
			break;
			
		case BACK:
			cube->face(TOP)->colors[0]=rnc[0];
			cube->face(TOP)->colors[1]=rnc[1];
			cube->face(TOP)->colors[2]=rnc[2];
			cube->face(LEFT)->colors[0]=rnc[3];
			cube->face(LEFT)->colors[1]=rnc[4];
			cube->face(LEFT)->colors[2]=rnc[5];
			cube->face(BOTTOM)->colors[2]=rnc[6];
			cube->face(BOTTOM)->colors[1]=rnc[7];
			cube->face(BOTTOM)->colors[0]=rnc[8];
			cube->face(RIGHT)->colors[2]=rnc[9];
			cube->face(RIGHT)->colors[1]=rnc[10];
			cube->face(RIGHT)->colors[0]=rnc[11];
			break;
		case NONE:
			break;
	}
}

void Face::glRotateFace(int i, Cube* c)
{
	const Face* face1 = 0;
	const Face* face2 = 0;

	switch (mDir)
	{
		case TOP:	// ERREUR, CE N'EST PLUS LA FACE TOP
					// IL FALLAIT REAFFECTER AUSSI LES FACES ELLES MEMES
			if (i<3)
				face1 = c->face(BACK);
			if (i>=6)
				face1 = c->face(FRONT);
			if (i==0||i==3||i==6)
				face2 = c->face(LEFT);
			if (i==2||i==5||i==8)
				face2 = c->face(RIGHT);
			break;

		case BOTTOM:
			if (i<3)
				face1 = c->face(BACK);
			if (i>=6)
				face1 = c->face(FRONT);
			if (i==0||i==3||i==6)
				face2 = c->face(LEFT);
			if (i==2||i==5||i==8)
				face2 = c->face(RIGHT);
			break;

		case BACK:
			if (i<3)
				face1 = c->face(TOP);
			if (i>=6)
				face1 = c->face(BOTTOM);
			if (i==0||i==3||i==6)
				face2 = c->face(LEFT);
			if (i==2||i==5||i==8)
				face2 = c->face(RIGHT); //glRotatef(90.0,1.0,0.0,0.0);

			break;

		case LEFT:
			if (i<3)
				face1 = c->face(BACK);
			if (i>=6)
				face1 = c->face(FRONT);
			if (i==0||i==3||i==6)
				face2 = c->face(BOTTOM);
			if (i==2||i==5||i==8)
				face2 = c->face(TOP);

			//glRotatef(90,0.0,0.0,1.0);
			break;

		case RIGHT:
			if (i<3)
				face1 = c->face(BACK);
			if (i>=6)
				face1 = c->face(FRONT);
			if (i==0||i==3||i==6)
				face2 = c->face(BOTTOM);
			if (i==2||i==5||i==8)
				face2 = c->face(TOP); //glRotatef(90,0.0,0.0,1.0);
			break;

		case FRONT:
			if (i<3)
				face1 = c->face(TOP);
			if (i>=6)
				face1 = c->face(BOTTOM);
			if (i==0||i==3||i==6)
				face2 = c->face(LEFT);
			if (i==2||i==5||i==8)
				face2 = c->face(RIGHT);
			break;

		default:
			break;

	}
	if (face1) face1->glRotateAngle();
	if (face2) face2->glRotateAngle();
}

void Face::glRotateFlat()
{
	switch (mDir)
	{
		case TOP:
			glRotatef(180.0, 1.0,0.0, 0.0);
		case BOTTOM:
			break;

		case BACK:
			glRotatef(180, 0.0, 0.0, 1.0);
			break;

		case LEFT:
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			break;

		case RIGHT:
			glRotatef(90, 0.0, 0.0, 1.0);
			break;

		case FRONT:
			glRotatef(180.0, 1.0, 0.0, 0.0);
			break;

		default:
			break;

	}
}

void Face::glRotate()
{
	switch (mDir)
	{
		case TOP:
		case BOTTOM:
			break;

		case BACK:
			glRotatef(90.0, 1.0, 0.0, 0.0);
			break;

		case LEFT:
			glRotatef(90, 0.0, 0.0, 1.0);
			break;

		case RIGHT:
			glRotatef(90, 0.0, 0.0, 1.0);
			break;

		case FRONT:
			glRotatef(90.0, 1.0, 0.0, 0.0);
			break;

		default:
			break;

	}
}

void Face::glTranslate()
{
	switch (mDir)
	{
		case TOP:
			glTranslatef(0, 1.5*Face::size, 0);
			break;

		case BOTTOM:
			glTranslatef(0, -1.5*Face::size, 0);
			break;

		case BACK:
			glTranslatef(0, 0, -1.5*Face::size);
			break;

		case LEFT:
			glTranslatef(-1.5*Face::size, 0, 0);
			break;

		case RIGHT:
			glTranslatef(1.5*Face::size, 0, 0);
			break;

		case FRONT:
			glTranslatef(0, 0, 1.5*Face::size);
			break;

		case NONE:
			break;
	}
}

void Face::rotate(float a)
{
	angle_target = a;
}

void Face::update(Cube* cube, float time)
{
	if (angle_target && (time>0))
	{
		float delta = fRotationSpeed*time;
		bool end = false;

		if (angle_target>0)
		{
			angle += delta;
			end = angle>=angle_target;
		}
		else
		{
			angle -= delta;
			end = angle<=angle_target;
		}

		if (end)
		{
			//cout<<"end "<<angle_target<<endl;
			// moveFacets()
			while (angle_target>=50)
			{
				//cout<<"  rotate false "<<angle_target<<endl;
				rotateFaces(false, cube);
				angle_target -= 90;
			}
			while (angle_target<=-50)
			{
				//cout<<"  rotate true "<<angle_target<<endl;
				rotateFaces(true, cube);
				angle_target += 90;
			}
			//cout<<"end"<<endl;
			angle = 0;
			angle_target = 0;
		}
	}
}

int Face::epsilon() const
{
	switch (mDir)
	{
		case TOP:
		case LEFT:
		case FRONT:
			return -1;

		default:
			return 1;
	}
}

string Face::dirToString(Dir d)
{
	switch(d)
	{
		case TOP:		return "top";
		case BOTTOM:	return "bottom";
		case LEFT:		return "left";
		case RIGHT:		return "right";
		case FRONT:		return "front";
		case BACK:		return "back";
		default:		return "unkown";
	}
}

string Face::desc(bool compact) const
{
	string desc;
	for(int i=0; i<9; i++)
	{
		if (compact)
			desc += colors[i].name()[0];
		else
		{
			if (desc.length()) desc +=' ';
			desc += colors[i].name();
		}
	}
	return desc;
}

void Face::setColor(int face, const Color& color)
{
	if (face>=0 && face <9)
	{
		colors[face]=color;
	}
}
