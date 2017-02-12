/***************************************************************************
	vim:tabstop=4
	*/
#ifndef	_Color_
#define	_Color_
#include <iostream>
#include <string>

using namespace std;

class Color
{
public:
	Color(double r,double v,double b, const string& name="")
		:
		mr		(r),
		mv		(v),
		mb		(b),
		ma		(1.0),
		mname	(name){ };
		
	Color(double r,double v,double b,double a)
		:
		mr		(r),
		mv		(v),
		mb		(b),
		ma		(a)	{};

	Color()
		:
		mr		(0.5),
		mv		(0.5),
		mb		(0.9),
		ma		(1.0)	{};

	Color(const Color& col,double a)
		:
		mr		(col.mr),
		mv		(col.mv),
		mb		(col.mb),
		ma		(a)	{};

	Color	operator*(const double f) const { return Color(mr*f,mv*f,mb*f);	};
	
			
	Color*	Duplicate()	const;
		
	void	render()			const ;
	void	render(float alpha)	const;

	double r() const { return mr; }
	double v() const { return mv; }
	double b() const { return mb; }
	double alpha() const { return ma; }
	
	bool operator!=(const Color& c) const { return !(*this==c); }
	bool operator==(const Color& c) const {
		return mr==c.mr && mv==c.mv && mb==c.mb && ma==c.ma;
	}
	
	void setName(const string& n) { mname=n; }
	const string& name() const { return mname; }
	
	// if data=random, random color is returned.
	static const Color* factory(string &data);
	static const Color* random();

private:
	double	mr;
	double	mv;
	double	mb;
	double	ma;
	string mname;

public:
	static const Color black;
	static const Color light_gray;
	static const Color dark_gray;
	static const Color gray;
	static const Color white;
	static const Color red	;
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

#endif
