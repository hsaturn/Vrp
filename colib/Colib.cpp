/* 
 * File:   Colib.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 2016, 20:47
 */

#include <GL/glew.h>

#include "Colib.hpp"
#include "StringUtil.hpp"
#include "Bati.hpp"

namespace Colib
{
	Colib::ColibBuilder Colib::builder;
	
	Colib::Colib(const string& name, string &incoming)
	: Object(name)
	{
		config = StringUtil::getLong(incoming);
		colCount = StringUtil::getLong(incoming);
		height = StringUtil::getFloat(incoming);

		cout << "INC " << config << ", " << colCount << ", " << height << endl;
		
		if (config<2) config=2;
		if (config>3) config=3;
		if (colCount<1) colCount=1;
		if (colCount>40) colCount=40;
		
		if (height<100)
			height = 100;
		if (height>500)
			height = 500;
		
		recalcSize();
		
		cout << "WIDTH : " << getWidth() << endl;
		cout << "HEIGHT : " << getHeight() << endl;
		cout << "LENGTH: " << getLength() << endl;
		cout << "COL_WIDTH: " << COL_WIDTH << ", CFG_WIDTH: " << CFG_WIDTH << endl;
		
		setVar("scale", "0.1");
		
		bati = new Bati(this);
		setVar("dx" ,StringUtil::to_string(-width/20));
		setVar("dz", StringUtil::to_string(-getLength()/20));
	}
	
	Colib::~Colib()
	{
	}

	void Colib::_render(bool resetTimer)
	{
		Color::dark_green.render();
		glBegin(GL_QUADS);
		glVertex3i(0,0,0);
		glVertex3i(width,0,0);
		glVertex3i(width, 0, length);
		glVertex3i(0, 0, length);
		glEnd();
		
		bati->render();
	}
	
	bool Colib::_execute(Server*, string cmd, string incoming, const string& org)
	{
		bool bRet = false;
		
		if (cmd=="add")
			bRet = true;
		else if (cmd=="read")
		{
		}
		return bRet;
	}
	
	void Colib::_help(Help& help)
	{
		//help.add("world.add block_type ...");
	}
	
	void Colib::recalcSize() {
		width = config * CFG_WIDTH+(config-1)*CLO_THICK;
		length = colCount*COL_WIDTH+(colCount-1)*CLO_THICK;
		cout << "RECALC SIZE CFG=" << config << ", W=" << width << ", L=" << length << endl;
	}
}


