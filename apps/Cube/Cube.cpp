#include <Cube.h>
#include <Color.h>
#include <Face.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <chrono>
#include <list>
#include "StringUtil.hpp"
#include "Random.hpp"
#include <iomanip>
#include <Widget.h>

using namespace std;

map<string, string> patterns;

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

Cube::CubeBuilder Cube::builder;

const float flatx_org = 2.5;
const float flaty_org = 4.8;
const float flatz_org = 4.0;
const float flats_org = 1.0;

Cube::Cube(const string& name, string& incoming) : Object(name) {
	fillPatterns();
	backward = true;
	
	// backward position & scale
	back_x = 4.1;
	back_y = 4.1;
	back_z = 2.1;
	back_s = 0.5;

	flat = true;
	flatx = flatx_org;
	flaty = flaty_org;
	flatz = flatz_org;
	flats = flats_org;

	reset();
}

void Cube::reset() {
	total_moves = 0;
	slearn = "";
	learn = false;

	faces[Face::TOP].reset(Face::TOP, Color::white);
	faces[Face::FRONT].reset(Face::FRONT, Color::blue);
	faces[Face::LEFT].reset(Face::LEFT, Color::red);
	faces[Face::RIGHT].reset(Face::RIGHT, Color::orange);
	faces[Face::BOTTOM].reset(Face::BOTTOM, Color::yellow);
	faces[Face::BACK].reset(Face::BACK, Color::green);

	anglex = 0;
	angley = 0;
	anglez = 0;
	anglex_target = 0;
	angley_target = 0;
	anglez_target = 0;


	moving = false;

	redir[Face::TOP] = Face::TOP;
	redir[Face::FRONT] = Face::FRONT;
	redir[Face::LEFT] = Face::LEFT;
	redir[Face::RIGHT] = Face::RIGHT;
	redir[Face::BOTTOM] = Face::BOTTOM;
	redir[Face::BACK] = Face::BACK;
}

void Cube::renderHud() {
	string state;
	if (!isValid())
	{
		Color::red.render();
		state = "invalid";
	}
	else
		state = "scrambled";
	
	if (isMade())
	{
		Color::green.render();
		state = "solved";
	}
	if (lastState != state)
	{
		hwidgets::Widget::pushEvent("cube", state);
		lastState = state;
	}
	stringstream buf;
	buf << "Moves: " << totalMoves();
	buf << ", " << (isValid() ? "valid" : "invalid");
	if (isMade()) buf << ", perfect";
	drawHudText(buf.str().c_str());
}

void Cube::renderFlat(float size) {
	glPushMatrix();
	glTranslatef(-size, 0, 0);
	faces[redir[Face::LEFT]].renderFlat(size, this);
	glTranslatef(size, size, 0.0);
	faces[redir[Face::TOP]].renderFlat(size, this);
	glTranslatef(0.0, -2 * size, 0.0);
	faces[redir[Face::BOTTOM]].renderFlat(size, this);
	glTranslatef(0.0, size, 0.0);
	faces[redir[Face::FRONT]].renderFlat(size, this);
	glTranslatef(size, 0.0, 0.0);
	faces[redir[Face::RIGHT]].renderFlat(size, this);
	glTranslatef(size, 0.0, 0.);
	faces[redir[Face::BACK]].renderFlat(size, this);
	glPopMatrix();
}

bool Cube::_render(bool resetTimer) {
	static long wait = 0;
	static int angle = 0;
	static auto t_start = std::chrono::high_resolution_clock::now();

	if (resetTimer) {
		t_start = std::chrono::high_resolution_clock::now();
	}
	auto t_end = std::chrono::high_resolution_clock::now();

	auto ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
	t_start = t_end;

	if (wait > 0)
		wait--;
	else {
		angle++;
		if (angle >= 360) angle = 0;
		if ((angle % 90) == 0)
			wait = 10;
	}

	float time = ms / 1000;

	moving = false;
	updateAngle(anglex_target, anglex, time);
	updateAngle(angley_target, angley, time);
	updateAngle(anglez_target, anglez, time);

	renderCube(time);
	
	if (backward)
	{
		glLoadIdentity();
		glRotatef(180, 0.0, 1.0, 0.0);
		//glMultMatrixf(glm::value_ptr(orient));
		glTranslatef(back_x, back_y, back_z);
		glScalef(back_s, back_s, back_s);
		glRotatef(-10, 1.0, 1.0, 0.0);
		renderCube(time);
	}
	if (flat)
	{
		glLoadIdentity();
		glTranslatef(flatx, flaty, flatz);
		glScalef(flats, flats, flats);
		renderFlat();
	}
	return !isReady();
}

void Cube::renderCube(float time)
{
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	//cout << "time_ms=" << ms << endl;
	for (int i = 0; i < 6; i++) faces[i].update(this, time);
	for (int i = 0; i < 6; i++) faces[i].render(this);
}

void Cube::updateAngle(int& target, float& angle, float time) {
	float delta = Face::getRotationSpeed() * time;
	int end = 0;

	if (angle < target) {
		moving = true;
		angle += delta;
		if (angle >= target) {
			if (areFacesReady())
				end = 1;
			else
				angle -= delta;
		}
	} else if (angle > target) {
		moving = true;
		angle -= delta;
		if (angle <= target) {
			if (areFacesReady())
				end = -1;
			else
				angle += delta;
		}
	}
	if (end) {
		cout << "--------END MOVE " << end << "---------" << endl;
		if (!areFacesReady())
			cout << "************* ERREUR FACES ARE NOT READY **************";
		list<Face::Dir> l;
		if (&angle == &anglex) {
			l.push_back(redir[Face::BACK]);
			l.push_back(redir[Face::BOTTOM]);
			l.push_back(redir[Face::FRONT]);
			l.push_back(redir[Face::TOP]);
		} else if (&angle == &angley) {
			l.push_back(redir[Face::RIGHT]);
			l.push_back(redir[Face::FRONT]);
			l.push_back(redir[Face::LEFT]);
			l.push_back(redir[Face::BACK]);
		} else if (&angle == &anglez) {
			l.push_back(redir[Face::TOP]);
			l.push_back(redir[Face::RIGHT]);
			l.push_back(redir[Face::BOTTOM]);
			l.push_back(redir[Face::LEFT]);
		}
		if (end > 0) {
			l.push_back(l.front());
			l.pop_front();
		} else {
			l.push_front(l.back());
			l.pop_back();
		}
		if (&angle == &anglex) {
			cout << "REDIR X " << end << endl;
			redir[Face::BACK] = l.front();
			l.pop_front();
			redir[Face::BOTTOM] = l.front();
			l.pop_front();
			redir[Face::FRONT] = l.front();
			l.pop_front();
			redir[Face::TOP] = l.front();
			l.pop_front();


			faces[redir[Face::BACK]].setDir(Face::BACK);
			faces[redir[Face::BOTTOM]].setDir(Face::BOTTOM);
			faces[redir[Face::FRONT]].setDir(Face::FRONT);
			faces[redir[Face::TOP]].setDir(Face::TOP);

			if (end > 0) {
				faces[redir[Face::TOP]].swapFaces();
				faces[redir[Face::BOTTOM]].swapFaces();
			} else {
				faces[redir[Face::FRONT]].swapFaces();
				faces[redir[Face::BACK]].swapFaces();
			}
			faces[redir[Face::LEFT]].rotateFaces(end < 0, 0);
			faces[redir[Face::RIGHT]].rotateFaces(end > 0, 0);

			//faces[redir[Face::TOP]].setDir(Face::TOP);
		} else if (&angle == &angley) {
			cout << "ENDY" << endl;
			redir[Face::RIGHT] = l.front();
			l.pop_front();
			redir[Face::FRONT] = l.front();
			l.pop_front();
			redir[Face::LEFT] = l.front();
			l.pop_front();
			redir[Face::BACK] = l.front();
			l.pop_front();

			faces[redir[Face::RIGHT]].setDir(Face::RIGHT);
			faces[redir[Face::FRONT]].setDir(Face::FRONT);
			faces[redir[Face::LEFT]].setDir(Face::LEFT);
			faces[redir[Face::BACK]].setDir(Face::BACK);

			faces[redir[Face::LEFT]].rotateFaces(false, 0);
			faces[redir[Face::TOP]].rotateFaces(end > 0, 0);
			faces[redir[Face::BACK]].rotateFaces(end < 0, 0);
			faces[redir[Face::RIGHT]].rotateFaces(true, 0);
			if (end > 0) {
				faces[redir[Face::LEFT]].swapFaces();
				faces[redir[Face::RIGHT]].swapFaces();
			} else {
				faces[redir[Face::FRONT]].swapFaces();
				faces[redir[Face::BACK]].swapFaces();
			}
			faces[redir[Face::FRONT]].rotateFaces(true, 0);
			faces[redir[Face::BOTTOM]].rotateFaces(end < 0, 0);
		} else if (&angle == &anglez) {
			cout << "ENDZ" << endl;
			redir[Face::TOP] = l.front();
			l.pop_front();
			redir[Face::RIGHT] = l.front();
			l.pop_front();
			redir[Face::BOTTOM] = l.front();
			l.pop_front();
			redir[Face::LEFT] = l.front();
			l.pop_front();

			faces[redir[Face::TOP]].setDir(Face::TOP);
			faces[redir[Face::RIGHT]].setDir(Face::RIGHT);
			faces[redir[Face::BOTTOM]].setDir(Face::BOTTOM);
			faces[redir[Face::LEFT]].setDir(Face::LEFT);

			faces[redir[Face::BACK]].rotateFaces(end < 0, 0);
			faces[redir[Face::FRONT]].rotateFaces(end > 0, 0);
			faces[redir[Face::RIGHT]].swapFaces();
			faces[redir[Face::RIGHT]].rotateFaces(true, 0);
			faces[redir[Face::RIGHT]].rotateFaces(true, 0);
			//faces[redir[Face::LEFT]].swapFaces(true);
			//faces[redir[Face::RIGHT]].rotateFaces(end>0,0);
			//faces[redir[Face::LEFT]].rotateFaces(end<0,0);
			if (end > 0) {
				faces[redir[Face::RIGHT]].swapFaces(true);
				faces[redir[Face::BOTTOM]].swapFaces(true);
				faces[redir[Face::TOP]].swapFaces(true);
			} else
				faces[redir[Face::LEFT]].swapFaces(true);

		}

		/*for (int d = (int) Face::TOP; d<Face::NONE; d++)
			faces[redir[d]].setDir(d);*/

		/*for (int d = (int) Face::TOP; d<Face::NONE; d++)
		{
			face(d)->setDir((Face::Dir)d);
		}*/

		cout << endl;
		angle = 0;
		target = 0;
	}
}

bool Cube::rotate(Face::Dir dir, float angle) {
	if (dir != Face::NONE) {
		faces[redir[dir]].rotate(angle);
		return true;
	} else
		return false;
}

bool Cube::isReady() const {
	if (moving)
		return false;

	return areFacesReady();
}

bool Cube::areFacesReady() const {
	bool ready = true;
	for (int i = 0; i < 6; i++ && ready)
		ready &= faces[i].isReady();
	return ready;
}

string Cube::find(const Color* center) const {
	for (int i = 0; i < 6; i++) {
		if (faces[i].get(4) == *center)
			return Face::dirToString((faces[i].mDir));
	}
	return "";
}

string Cube::find(const Color* c1, const Color* c2) const {
	for (int i = 0; i < 6; i++) {
		string where = faces[i].find(c1, c2, this);
		if (where.length())
			return where;
	}
	return "";
}

string Cube::find(const Color* c1, const Color* c2, const Color* c3) const {
	for (int i = 0; i < 6; i++) {
		string where = faces[i].find(c1, c2, c3, this);
		if (where.length())
			return where;
	}
	return "";
}

void Cube::rotatex(bool clockwise) {
	cout << "rotate x;" << endl;
	if (clockwise)
		anglex_target -= 90;
	else
		anglex_target += 90;
	moving = true;
}

void Cube::rotatey(bool clockwise) {
	if (clockwise)
		angley_target -= 90;
	else
		angley_target += 90;
	moving = true;
}

void Cube::rotatez(bool clockwise) {
	if (clockwise)
		anglez_target += 90;
	else
		anglez_target -= 90;
	moving = true;
}

bool Cube::isMade() const {
	bool bMade = true;
	for (int i = 0; i < 6; i++)
		bMade = bMade && faces[i].isUniform();
	return bMade;
}

bool Cube::isValid() const {
	return has(Color::white) && has(Color::blue) && has(Color::orange) &&
		has(Color::yellow) && has(Color::red) && has(Color::green) &&

		has(Color::white, Color::blue) && has(Color::white, Color::orange) &&
		has(Color::white, Color::green) && has(Color::white, Color::red) &&

		has(Color::yellow, Color::blue) && has(Color::yellow, Color::orange) &&
		has(Color::yellow, Color::green) && has(Color::yellow, Color::red) &&

		has(Color::white, Color::blue, Color::orange) &&
		has(Color::white, Color::blue, Color::red) &&
		has(Color::white, Color::green, Color::orange) &&
		has(Color::white, Color::green, Color::red) &&

		has(Color::yellow, Color::blue, Color::red) &&
		has(Color::yellow, Color::blue, Color::orange) &&
		has(Color::yellow, Color::green, Color::orange) &&
		has(Color::yellow, Color::green, Color::red);
}

bool Cube::has(const Color& c) const {
	return find(&c).length() != 0;
}

bool Cube::has(const Color& c1, const Color& c2) const {
	return find(&c1, &c2).length() != 0;
}

bool Cube::has(const Color& c1, const Color& c2, const Color &c3) const {
	return find(&c1, &c2, &c3).length() != 0;
}

string Cube::desc(Face::Dir d) const {
	if (d >= Face::TOP && d <= Face::BACK) {
		return faces[redir[d]].desc();
	}
	return "?";
}

string Cube::getColors(char sep) const {
	string colors;
	for (int face = 0; face < 6; face++) {
		colors += faces[redir[face]].desc(true);
		if (sep != 0 && face != 5)
			colors += sep;
	}
	while (colors.find('d') != string::npos)
		colors[colors.find('d')] = '.';
	return colors;
}

bool Cube::setColors(const string& colors) {
	bool ret = true;

	if (colors.length() == 54 || colors.length() == 59) {
		int col = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 9; j++) {
				char c = colors[col++];
				c |= 32;
				Color color;
				switch (c) {
					case 'w': color = Color::white;
						break;
					case 'b': color = Color::blue;
						break;
					case 'o': color = Color::orange;
						break;
					case 'g': color = Color::green;
						break;
					case 'r': color = Color::red;
						break;
					case 'y': color = Color::yellow;
						break;
					default: color = Color::dark_gray;
						break;
				}
				faces[redir[i]].setColor(j, color);
			}
			if (colors.length() == 59) col++;
		}
	}
	return ret;
}

bool Cube::check(const Color* c, string& s) {
	string where = find(c);
	string found = getWord(s);
	cout << "check where=" << where << " found=" << found << endl;
	if (where.length() == 0) return false;
	if (where[0] != found[0]) return false;
	return true;
}

list<string> stack; // stack of colors

void Cube::_help(Help& help) {
	help.add("algo rotate {axes} {algo}");
	help.add("backward [x y z [scale]]");
	help.add("color dir                  (dir=top bottom left right front back");
	help.add("colors [colorlist]");
	help.add("desc dir");
	help.add("find color [color [color]] (find center/edge/corner)");
	help.add("front color");
	help.add("is_made, is the cube made ?");
	help.add("is_valid [why], is the cube in a valid configuration");
	help.add("orient color [color] orient cube");
	help.add("peek/pop/push [name]/stack remember stack of cubes");
	help.add("mark");
	help.add("move m[m...]               (m=lrtdfb['][2..9])");
	help.add("orient color1[ color2]     (color1 to top, color2 to front)");
	help.add("rotate dir angle ...       (dir=top bottom left right front back x y z, angle=deg)");
	help.add("shuffle [seed]");
	help.add("top color");
	help.add("rspeed  n                  (deg/sec)");
	help.add("reset");
	
	stringstream builder;
	for(auto s: patterns)
		builder << s.first << ' ';
	help.add(builder.str());
}

Face::Dir Cube::getDir(string& incoming, string& dir)
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

Object::ExecResult Cube::_execute(Server* server, string cmd, string incoming, const string& org, CmdQueue& cmdQueue) {
	if (!isReady())
		return EXEC_BUSY;
	
	ExecResult ret = EXEC_OK;
	
	string prefix=getName()+'.';
	string prefixa='@'+getName()+'.';

	if (cmd == "is_made")
	{
		if (isMade())
			server->send("#OK is_made CUBE IS PERFECT !");
		else
			server->send("#KO is_made");
	}
	else if (cmd == "top")
	{
		bool ok = true;
		const Color* top = getColor(incoming);
		if (top)
		{
			string face = find(top);
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
	else if (cmd == "reset")
	{
		reset();
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

			string face = find(c1); // c1 to top
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
			string face = find(c1); // c1 to front

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
	else if (cmd == "front")
	{
		bool ok = true;
		const Color* front = getColor(incoming);
		if (front)
		{
			string face = find(front);
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
	else if (cmd == "is_valid")
	{
		if (isValid())
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
				back_x = eatFloat(incoming);
				if (incoming.length()) back_y = eatFloat(incoming);
				if (incoming.length()) back_z = eatFloat(incoming);
				if (incoming.length()) back_s = eatFloat(incoming);
			}
		}
		else
			backward = !backward;
		stringstream buf;
		buf << setprecision(2) << "#OK backward=(" << back_x << ',' << back_y << ',' << back_z << ") scale=" << back_s;
		server->send(buf.str());
	}
	else if (cmd == "color")
	{
		string dir;
		Face::Dir d = getDir(incoming, dir);

		if (d != Face::NONE)
			server->send("#OK " + face(d)->get(4).name());
		else
			server->send("#KO invalid direction " + dir);
	}
	else if (cmd == "desc")
	{
		string dir;
		Face::Dir d = getDir(incoming, dir);
		if (d != Face::NONE)
		{
			string dsc = desc(d);
			server->send("#OK desc=" + dsc);
		}
		else
			server->send("#KO desc");
	}
	else  if (cmd == "flat")
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
				flatx = eatFloat(incoming);
				if (incoming.length()) flaty = eatFloat(incoming);
				if (incoming.length()) flatz = eatFloat(incoming);
				if (incoming.length()) flats = eatFloat(incoming);
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
	else if (cmd == "rotate")
	{
		StringUtil::trim(incoming);

		string invalid;
		while (incoming.length())
		{
			string dir;
			Face::Dir face = getDir(incoming, dir);
			float angle = eatFloat(incoming);

			if (face != Face::NONE)
				rotate(face, angle);
			else
			{
				bool clockwise = false;
				if (angle < 0) clockwise = true;
				if (dir == "x")
					rotatex(clockwise);
				else if (dir == "y")
					rotatey(clockwise);
				else if (dir == "z")
					rotatez(clockwise);
				else
					invalid += dir;
			}
		}
		if (invalid.length())
			server->send("#ERROR: Invalid direction [" + invalid + "]");
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
			where = find(c1);
		else if (c3 == 0)
			where = find(c1, c2);
		else
			where = find(c1, c2, c3);
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
	else if (cmd == "is_ok") {
		const Color* c1 = getColor(incoming);
		const Color* c2 = 0;
		const Color* c3 = 0;
		if (incoming.length()) c2 = getColor(incoming);
		if (incoming.length()) c3 = getColor(incoming);

		string where;

		cout << "find " << c1 << " " << c2 << " " << c3 << endl;

		if (c1 == 0)
			server->send("#KO");
		else if (c2 == 0) {
			where = find(c1);
		} else if (c3 == 0) {
			where = find(c1, c2);
		} else {
			where = find(c1, c2, c3);
		}
		if (where.length()) {
			string where2(where);
			cout << "found (" << where << ")" << endl;

			//server->send("#OK " + org + "=" + where);
			bool bOk = true;
			if (c1) bOk &= check(c1, where);
			if (c2) bOk &= check(c2, where);
			if (c3) bOk &= check(c3, where);
			if (bOk)
				server->send("#OK " + org + "=" + where2);
			else
				server->send("#KO " + org + "=" + where2);
		} else {
			cout << "not found !" << endl;
			server->send("#KO " + org);
		}
		cout << "---------------" << endl;

	} else if (cmd == "push") {
		stack.push_back(StringUtil::to_string(total_moves) + " " + getColors() + " " + incoming);
		server->send("#OK push, stk_size=" + StringUtil::to_string(stack.size()));
	} else if (cmd == "stack") {
		if (incoming == "clear")
			cmdQueue.clear();
		else {
			server->send("-----");
			int count = stack.size();
			for (auto it = stack.begin(); it != stack.end(); it++)
				server->send(StringUtil::to_string(--count) + " : " + *it);
			server->send("-----");
			server->send("size: " + StringUtil::to_string(stack.size()));
		}
	} else if (cmd == "pop" || cmd == "peek") {
		if (stack.size()) {
			string pop = stack.back();
			total_moves = atol(getWord(pop).c_str());
			setColors(getWord(pop));
			if (cmd == "pop") stack.pop_back();
			server->send("#OK " + cmd + ", stk_size=" + StringUtil::to_string(stack.size()));
		} else
			server->send("#KO " + cmd + ": empty stack");
	} else if (cmd == "_reset_counter") {
		total_moves = 0;
	} else if (cmd == "colors") {
		char sep;
		bool bOk = true;
		StringUtil::trim(incoming);
		string colors;
		if (incoming.length() > 1) {
			colors = getWord(incoming);

			reset();
			bOk = setColors(colors);
		} else {
			colors = getColors();
			sep = incoming[0];
		}
		long key = 0;
		int shift = 0;
		for (auto it = colors.begin(); it != colors.end(); it++) {
			shift++;
			if (shift > 16) shift = 0;
			char c = *it;
			key += c + (c << shift)+(c^257);
		}
		key = key % 1000;
		if (bOk)
			server->send("#OK colors = " + getColors(sep) + " key=" + StringUtil::to_string(key));
		else
			server->send("#KO colors (length must be 54");
	} else if (cmd == "shuffle") {

		string what = incoming;
		string number = "";

		static int seed = 0;
		bool reset = true;

		if (what == "next")
			seed++;
		else if (what.length())
			seed = atol(incoming.c_str());
		else {
			reset = false;
			seed = time(NULL);
		}

		Random::seed(seed);
		number = " cube #" + StringUtil::to_string(seed) + " ";
		int moves = 16 + Random::rand() % 10;

		string cmd = "move ";
		string f = "lrtdfbems";
		char lastm = 0;
		while (moves > 0) {
			char newm;
			while ((newm = f[Random::rand() % 9]) == lastm);
			lastm = newm;
			cmd += newm;
			if (Random::rand()&1)
				cmd += '\'';
			moves--;
		}
		cmdQueue.push_front("@_reset_counter");
		cmdQueue.push_front('@' + cmd);
		if (reset) cmdQueue.push_front("@reset");
		server->send("#OK " + number + cmd);
	} else if (cmd == "move" || cmd == "reverse") {
		int local_moves = 0;
		bool bReverse = cmd == "reverse";
		bool bAlgo = incoming.substr(0, 4) == "algo";
		bool bAlgo2 = false;
		if (bAlgo) getWord(incoming);
		if (incoming.substr(0, 5) == "learn")
			incoming = slearn;
		string moves = incoming;
		string sReverse;
		string sReverse2;
		list<string> cmdInt;
		while (incoming.length()) {
			string move = "";
			string face;
			char c = incoming[0];
			if (c >= 'A' && c <= 'Z') c += 32;
			incoming.erase(0, 1);
			if (c == 'u') face = "top";
				// http://ruwix.com/the-rubiks-cube/notation/
			else if (c == 'd') face = "bottom";
			else if (c == 'l') face = "left";
			else if (c == 'r') face = "right";
			else if (c == 't' || c == 'u') face = "top";
			else if (c == 'f') face = "front";
			else if (c == 'b') face = "back";
			else if (c == 'm') face = "right left x"; // + rotate cube
			else if (c == 'e') face = "top bottom y"; // + rotate cube
			else if (c == 's') face = "back front z"; // + rotate cube

			if (bAlgo && c == ' ') {
				bAlgo2 = true;
			}
			if (face.length()) {
				move += c;
				local_moves++;
				int angle = 90;
				if (bReverse)
					angle = -90;
				bool loop = true;
				while (loop && incoming.length()) {
					loop = false;
					char c = incoming[0];
					if (c == '\'') {
						move += c;
						loop = true;
						angle *= -1;
					}
					if (c >= '0' && c <= '9') {
						move += c;
						loop = true;
						angle *= (c - '0');
					}
					if (loop) incoming.erase(0, 1);
				}

				if (bAlgo) {
					if (move.find('\'') == string::npos)
						move += '\'';
					else
						move.erase(move.find('\'', 1));
					if (bAlgo2)
						sReverse2 = move + sReverse2;
					else
						sReverse = move + sReverse;
				}

				string cmd = string("@rotate ");
				while (face.length()) {
					string f = getWord(face);
					cmd += f + " " + StringUtil::to_string(angle) + " ";
					angle = -angle;
				}
				if (bReverse)
					cmdInt.push_front('@' + cmd);
				else
					cmdInt.push_back(cmd);
				cout << "pushing cmd [" << cmd << "]" << endl;
			} else if (c == ' ') {
			} else
				server->send(string("#ERROR Unknown face [") + StringUtil::to_string((int) c) + "]");
		}
		if (bAlgo) {
			string algo = moves + sReverse + sReverse2;
			while (algo.find(' ') != string::npos) algo.erase(algo.find(' '), 1);
			server->send("# ALGO " + algo);
			cmdQueue.push_front("@move " + algo);
		} else {
			total_moves += local_moves;
			server->send("#OK move (" + StringUtil::to_string(local_moves) + "/" + StringUtil::to_string(total_moves) + ") ");
			cmdQueue.push_front("@is_valid");
			while (cmdInt.size()) {
				string cmd = cmdInt.back();
				cmdQueue.push_front('@' + cmd);
				cmdInt.pop_back();
			}
		}
		if (learn) {
			slearn += moves;
		}
	} else if (cmd == "mark") {
		Face::toggleMark();
	} else if (cmd == "learn") {
		if (incoming == "end") {
			learn = false;
			server->send("#OK learn " + slearn);
		} else {
			learn = true;
			slearn = "";
		}
	} else if (cmd == "rspeed") {
		float f = atof(incoming.c_str());
		if (f >= 10) {
			Face::setRotationSpeed(f);
			server->send("#OK rspeed");
		} else
			server->send("#KO rspeed");
	}
	else if (patterns.find(cmd) != patterns.end())
	{
		cmdQueue.push_front("@move " + patterns[cmd]);
	}
	else
		ret = EXEC_UNKNOWN;

	return ret;
}

