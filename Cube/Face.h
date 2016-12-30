#ifndef Face_H
#define Face_H
#include <Color.h>

class Cube;

class Face
{
	public:
		enum Dir
		{
			TOP,
			BOTTOM,
			LEFT,
			RIGHT,
			FRONT,
			BACK,
			NONE
		};

		const Color& get(int i) const { return colors[i]; }

	public:
		Face();

		void reset(Dir, Color);
		void fill(const Color);
		
		void update(Cube*, float time);
		void render(Cube*);
		void renderFlat(float size, Cube*);
		
		bool isReady() const { return angle_target==0; }
		
		void setDir(Dir d) { mDir = d; }
		void setColor(int face, const Color& color);
		
		string desc(bool compact=false) const;
		
		Dir mDir;
		
		float angle;
		
		// Angle de destination
		int angle_target;
		
		void rotate(float angle);
		
		void rotateFaces(bool clockWise, Cube*);
		void swapFaces(bool left_right=false);
		
		static string dirToString(Dir);
		static void setRotationSpeed(float speed) { fRotationSpeed=speed; }
		static float getRotationSpeed() { return fRotationSpeed; }
		
		static float fRotationSpeed;
		
		// find a slice
		string find(const Color* c1, const Color* c2, const Cube* cube) const;
		
		// find a corner
		string find(const Color* c1, const Color* c2, const Color* c3, const Cube* cube) const;
		
		bool isUniform() const;
		
		static void toggleMark() { mark=!mark; }
	private:
		Color colors[9];
		static const float size;
		static bool mark;
		
		/* Rotation de la face (angle)*/
		void glRotateAngle() const;
		
		/* Rotation selon la direction de la face
		 * c=0 si inhiber la rotation y et x */
		void glRotate();
		void glRotateFlat();
		
		/* Rotation selon la/les faces adjacentes */
		void glRotateFace(int iface, Cube*);
		
		/* Translate selon la direction de la face*/
		void glTranslate();
		
		/* Retourne l'epsilone pour la case noir sous une facette */
		int epsilon() const;
		
		void fillAdjacentArray(Color* nc, Dir*, const Cube*) const;
};

#endif
