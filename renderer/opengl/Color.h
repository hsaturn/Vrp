/***************************************************************************
	vim:tabstop=4
 */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <GL/glew.h>

using namespace std;

class Color
{

	typedef struct color
	{
		GLfloat r;
		GLfloat v;
		GLfloat b;
		GLfloat alpha;
	} color;
  public:

	Color(GLfloat r, GLfloat v, GLfloat b, const string& name = "")
	:
	mr(r),
	mv(v),
	mb(b),
	ma(1.0),
	mname(name),
	stored(false) { };

	Color(GLfloat r, GLfloat v, GLfloat b, GLfloat a)
	:
	mr(r),
	mv(v),
	mb(b),
	ma(a),
	stored(false) { };

	Color()
	:
	mr(0.5),
	mv(0.5),
	mb(0.9),
	ma(1.0),
	stored(false) { };

	Color(const Color& col, GLfloat a)
	:
	mr(col.mr),
	mv(col.mv),
	mb(col.mb),
	ma(a),
	stored(false) { };

	~Color();

	Color operator *(const GLfloat f)const
	{
		return Color(mr*f, mv*f, mb * f);
	};


	Color* Duplicate() const;

	void render() const;
	void render(float alpha) const;
	void renderHighlight(float high) const;
	void renderHighlight(float high, float alpha) const;

	GLfloat r() const
	{
		return mr;
	}

	GLfloat v() const
	{
		return mv;
	}

	GLfloat b() const
	{
		return mb;
	}

	GLfloat alpha() const
	{
		return ma;
	}

	bool operator !=(const Color& c) const
	{
		return !(*this == c);
	}

	bool operator ==(const Color& c) const
	{
		return mr == c.mr && mv == c.mv && mb == c.mb && ma == c.ma;
	}

	void setName(const string& n)
	{
		mname = n;
	}

	const string& name() const
	{
		return mname;
	}

	// if data=random, random color is returned.
	static const Color* factory(string &data);
	static const Color* random();
	static map<string, const Color*> mapNamed;

  private:
	GLfloat mr;
	GLfloat mv;
	GLfloat mb;
	GLfloat ma;
	string mname;
	bool stored;

  public:
	static const Color black;
	static const Color light_gray;
	static const Color dark_gray;
	static const Color gray;
	static const Color white;
	static const Color red;
	static const Color green;
	static const Color yellow;
	static const Color blue;
	static const Color orange;
	static const Color cyan;
	static const Color magenta;
	static const Color dark_cyan;
	static const Color dark_red;
	static const Color dark_green;
	static const Color brown;

};

