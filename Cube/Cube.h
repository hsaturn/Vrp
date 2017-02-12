#ifndef Cube_H
#define Cube_H

#include "Face.h"
#include <string>

#include <Object.h>
#include <ObjectBuilder.hpp>

using namespace std;

class Cube : public Object {

	class CubeBuilder : public ObjectBuilder {
	public:

		CubeBuilder() : ObjectBuilder("cube") {}

		virtual ~CubeBuilder() {};

		Object* build(const string& name, string& incoming) {
			return new Cube(name, incoming);
		}
	};

public:
	Cube(const string& name, string& incoming);

	virtual ~Cube() {
	}


	void renderFlat(float size = 0.7);

	Face* face(int i) {
		return &faces[redir[i]];
	}

	const Face* face(int i) const {
		return &faces[redir[i]];
	}

	bool rotate(Face::Dir, float angle);

	void rotatex(bool clockwise);
	void rotatey(bool clockwise);
	void rotatez(bool clockwise);

	bool check(const Color* c, string& s);

	/* does any face is moving yet */

	bool areFacesReady() const;

	/* Returns true if cube is not moving and faces are not moving*/
	bool isReady() const;

	void reset();

	// find center
	string find(const Color*) const;

	// find slice
	string find(const Color*, const Color*) const;

	// find corner
	string find(const Color*, const Color*, const Color*) const;

	bool isValid() const;
	bool isMade() const;

	string desc(Face::Dir) const;

	string getColors(char sep = 0) const;
	bool setColors(const string& colors);

	void reboot();

	int totalMoves() const {
		return total_moves;
	}

	string learned() const {
		return slearn;
	}
	
	Face::Dir getDir(string& incoming, string& dir);
	
	virtual void renderHud();
	
protected:
	virtual void _help(Help&);
	virtual ExecResult _execute(Server* server, string cmd, string incoming, const string& org, CmdQueue&);
	virtual bool _render(bool resetTimer);
	
private:
	bool has(const Color&) const;
	bool has(const Color&, const Color&) const;
	bool has(const Color&, const Color&, const Color&) const;

	void updateAngle(int& target, float& angle, float timems);
	
	void renderCube(float time);

	Face faces[6];

	float anglex;
	float angley;
	float anglez;

	int anglex_target;
	int angley_target;
	int anglez_target;

	bool moving;

	Face::Dir redir[6];

	int total_moves;
	string slearn;
	bool learn;
	
	// display vars
	
	bool backward;
	float back_x, back_y, back_z, back_s;
	
	bool flat;
	float flatx, flaty, flatz, flats;


	static CubeBuilder builder;
	string lastState;
};

#endif
