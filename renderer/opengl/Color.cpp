#include	"Color.h"
#include "StringUtil.hpp"
#include <GL/glut.h>
#include "Widget.h"
#include <vector>
#include "Random.hpp"

using namespace hwidgets;

const Color Color::black		(0.0,0.0,0.0,"black");
const Color Color::light_gray	(0.7,0.7,0.7,"light_gray");
const Color Color::dark_gray	(0.2,0.2,0.2,"dark_gray");
const Color Color::gray			(0.5,0.5,0.5,"gray");
const Color Color::dark_cyan	(0.0,0.2,0.2,"dark_cyan");
const Color Color::dark_red		(0.2,0.0,0.0,"dark_red");
const Color Color::white		(1.0,1.0,0.8,"white");
const Color Color::red			(1.0,0.0,0.0,"red");
const Color Color::green		(0.0,1.0,0.0,"green");
const Color Color::dark_green	(0.0,0.5,0.0,"dark_green");
const Color Color::yellow		(1.0,1.0,0.0,"yellow");
const Color Color::blue			(0.0,0.0,1.0,"blue");
const Color Color::orange		(1.0,0.6,0.0,"orange");
const Color Color::cyan			(0.0,1.0,1.0,"cyan");
const Color Color::magenta		(1.0,0.0,1.0,"magenta");
const Color Color::brown		(75.0/256.0, 54.0/256.0, 33.0/256.0, "brown");

const Color* Color::random()
{
	static vector<const Color*> colors;
	if (colors.size()==0)
	{
		colors.push_back(&black);
		colors.push_back(&dark_gray);
		colors.push_back(&light_gray);
		colors.push_back(&gray);
		colors.push_back(&dark_cyan);
		colors.push_back(&dark_red);
		colors.push_back(&white);
		colors.push_back(&red);
		colors.push_back(&green);
		colors.push_back(&yellow);
		colors.push_back(&blue);
		colors.push_back(&orange);
		colors.push_back(&cyan);
		colors.push_back(&magenta);
		colors.push_back(&dark_green);
		colors.push_back(&brown);
	}
	
	int i=Random::rand()%colors.size();
	return colors[i];
}
void	Color::render() const
{
	glColor4f(mr,mv,mb,ma);
};

void Color::render(float alpha) const
{
	glColor4f(mr,mv,mb,alpha);
}

Color*	Color::Duplicate()	const
{
	return	new Color(mr,mv,mb,ma);
}

Color::~Color()
{
	if (stored)
	{
		for(auto it=mapNamed.begin(); it!=mapNamed.end(); it++)
			if (it->second==this)
			{
				mapNamed.erase(it);
				break;
			}
	}
}

float getHexFloat(string &data, int count=1)
{
	int value=0;
	bool shift=count==1;
	while(data.length() && count)
	{
		char c=data[0]|32;
		if (c>='a')
			c = c-'a'+10;
		else
			c -= '0';
		value = (value<<4)+c;
		data.erase(0,1);
		count--;
	}
	if (shift) value <<=4;
	return (float)value/256.0;
}

map<string,const Color*>	Color::mapNamed;

const Color* Color::factory(string& data)
{
	string color=StringUtil::getWord(data);
	if (color.length()==0) return 0;
	
	
	Color* newc = 0;
	static const Color* c=0;
	
	if (color=="help")
	{
		c = 0;
		Widget::pushMessage("color help");
		Widget::pushMessage("  [name=]#rvb[a]");
		Widget::pushMessage("  [name=]#rrvvbb[aa]");
		Widget::pushMessage("  [name=]r v b");
		Widget::pushMessage("  [name=]named_color");
	}
	else if (color!="last")
	{
		string named;
		if (color.find('=')!=string::npos) named=StringUtil::getWord(color,"=");
		
		c = 0;
		if (color == "white" || color == "w") c = &Color::white;
		else if (color == "blue" || color == "b") c = &Color::blue;
		else if (color == "green" || color == "g") c = &Color::green;
		else if (color == "yellow" || color == "y") c = &Color::yellow;
		else if (color == "orange" || color == "o") c = &Color::orange;
		else if (color == "red" || color == "r") c = &Color::red;
		else if (color == "cyan") c=&Color::cyan;
		else if (mapNamed.find(color)!=mapNamed.end())
		{
			c = mapNamed[color];
		}
		else if (color[0]=='#')
		{
			color.erase(0,1);
			int size=1;
			if (color.length()==6 || color.length()==8)
				size=2;
			else if (color.length()!=3 && color.length()!=4)
				return 0;
			
			newc = new Color;
			newc->mr=getHexFloat(color,size);
			newc->mv=getHexFloat(color,size);
			newc->mb=getHexFloat(color,size);
			if (color.length())
				newc->ma=getHexFloat(color,size);
			else
				newc->ma=1.0;

			c=newc;
		}
		else
		{
			newc = new Color;
			newc->mr=StringUtil::getFloat(color);
			newc->mv=StringUtil::getFloat(data);
			newc->mb=StringUtil::getFloat(data);

			if (StringUtil::startsWithFloat(data))
				newc->ma=StringUtil::getFloat(data);
			else
				newc->ma=1.0;

			c=newc;
		}
		if (c!=0 && named.length())
		{
			if (newc)
				newc->stored=true;
			mapNamed[named]=c;
		}
	}
	return c;
}
