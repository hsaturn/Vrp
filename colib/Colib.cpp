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
#include "Navette.hpp"
#include "Column.hpp"
#include "Cloison.hpp"

namespace Colib
{
	Colib::ColibBuilder Colib::builder;
	
	Colib::Colib(const string& name, string &incoming)
	: Object(name)
	{
		height = StringUtil::getFloat(incoming);

		if (height<100)
			height = 100;
		if (height>500)
			height = 500;
		
		recalcSize();
		
		setVar("scale", "0.1");
		
		bati = new Bati(this);
		setVar("dx" ,StringUtil::to_string(-width/20));
		setVar("dz", StringUtil::to_string(-getLength()/20));
	}
	
	Colib::~Colib()
	{
	}

	bool Colib::_render(bool resetTimer)
	{
		Color::dark_green.render();
		glBegin(GL_TRIANGLE_FAN);
		glNormal3i(0, 1, 0);
		glVertex3f(0,-0.01,0);
		glVertex3f(width,-0.01,0);
		glVertex3f(width, -0.01, length);
		glVertex3f(0, -0.01, length);
		glEnd();
		
		renderColumns(columns_back, 0);
		renderColumns(columns_front, 2*(Column::DEPTH + Cloison::THICKNESS));
		
		return bati->render();
	}
	
	void Colib::renderColumns(vector<Column*>& columns, int x1)
	{
		if (columns.size())
		{
			int x2 = x1 + Column::DEPTH;
			int z=Bati::THICKNESS;
			
			Cloison::render(x1, x2, getHeight(), z, heights);
			
			z+=Cloison::THICKNESS;
			for(auto column: columns)
			{
				column->render(x1,x2,z);
				z += column->getWidth();
				Cloison::render(x1, x2, getHeight(), z, heights);
				z += Cloison::THICKNESS;
			}
			
			if (last_frozen)
			{
				Color::blue.render();	// Separation chaud / froid
				glBegin(GL_TRIANGLE_STRIP);
				float h = getHeight(last_frozen)-0.01;
				glVertex3f(x1, h, 0);
				glVertex3f(x2, h, 0);
				glVertex3f(x1, h, length);
				glVertex3f(x2, h, length);
				glEnd();
			}
			
			// Jupes et tablier
			{
				int h = getHeight(0);
				Color::dark_gray.render();
				glBegin(GL_TRIANGLE_STRIP);
				glVertex3i(x1, 0, 0);
				glVertex3i(x1, 0, length);
				glVertex3i(x1, h, 0);
				glVertex3i(x1, h, length);
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);
				glVertex3i(x2, 0, 0);
				glVertex3i(x2, 0, length);
				glVertex3i(x2, h, 0);
				glVertex3i(x2, h, length);
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);
				
				float hf = h-0.01;
				glVertex3f(x1, hf, 0);
				glVertex3f(x2, hf, 0);
				glVertex3f(x1, hf, length);
				glVertex3f(x2, hf, length);
				glEnd();
				
			}
		}
	}

	
	bool Colib::_execute(Server* server, string cmd, string incoming, const string& org, CmdQueue&)
	{
		string error="";
		bool bRet = true;
		
		if (cmd=="go")
		{
			int z = StringUtil::getLong(incoming);	// Colonne
			int h = StringUtil::getLong(incoming);	// Cellule verticale
			
			if (getHeight(h)==-1)
			{
				cout << "ERREUR cellule" << endl;
				return true;
			}
			
			if (getCenterOfColumnZ(z)==-1)
			{
				cout << "ERREUR column";
				return true;
			}
			
			bati->moveTo(z,h);
			cout << "MOVE TO " << z << 'x' << h << endl;
		}
		else if (cmd=="height")
		{
			height = StringUtil::getLong(incoming);
			if (height<100)
				height=100;
			else if (height>2000)
				height = 2000;
			recalcSize();
		}
		else if (cmd=="fill")
		{
			if (bati->isReady())
			{
				if (bati->getPlateau())
					error="Not empty !";
				else
				{
					Plateau* plateau = new Plateau(incoming, (bati->getXLeft()+bati->getXRight())/2);
					bati->setPlateau(plateau);
				}
			}
			else
				error="Not ready !";
		}
		else if (cmd=="add")
		{
			string err="";
			bool help(false);
			if (incoming=="help" || incoming=="")
				help = true;
			else
			{
				vector<Column*>*	pcols = 0;
				string where = StringUtil::getIdentifier(incoming);
				if (where=="back")
					pcols = &columns_back;
				else if (where=="front")
					pcols = &columns_front;
				if (pcols)
				{
					int width = StringUtil::getLong(incoming);
					
					if (width<20 || width>100)
						help=true;
					else
					{
						Column* column = new Column(this, width);
						pcols->push_back(column);
						recalcSize();
						server->send("Column added");
					}
				}
				else
				{
					help=true;
					err="Missing back/front";
				}
			}
			
			if (help)
			{
				cerr << "Syntax error :-( " << err << endl;
				server->send("colib.add [back|front] width (cm), add a column. "+err);
			}
		}
		else if (cmd=="read")
		{
		}
		else if (cmd=="put")
		{
			StringUtil::trim(incoming);
			const char* err= bati->put(incoming!="front");
			if (err)
				error=err;
		}
		else if (cmd=="drop" || cmd=="remove")
		{
			bati->remove();
		}
		else if (cmd=="get")
		{
			StringUtil::trim(incoming);
			const char* err=bati->get(incoming!="front");
			if (err)
				error=err;
		}
		else
			bRet = false;
		
		if (error.length())
		{
			server->send("ERROR : " + error);
			cerr << "ERROR : " << error << endl;
			cout << "ERROR : " << error << endl;
			return false;
		}
		return bRet;
	}
	
	void Colib::_help(Help& help)
	{
		help.add("go column, height%");
		//help.add("world.add block_type ...");
	}
	
	Column* Colib::getColumn(unsigned int nr, bool back) const
	{
		const vector<Column*>* p = (back ? &columns_back : &columns_front);
		if (nr<p->size())
			return (*p)[nr];
		cerr << "Bad column number " << nr << " for " << back << " size=" <<  p->size() << endl;
		return 0;
	}
	
	int computeLength(vector<Column*>& columns)
	{
		if (columns.size()==0)
			return 0;
		
		int length= 0;
		for(auto column : columns)
			length += column->getWidth();
		
		length += 2*Cloison::THICKNESS;
		if (columns.size() > 1)
			length += Cloison::THICKNESS * (columns.size()-1);
				
		return length;
	}
	
	int Colib::getHeight(unsigned int etage) const
	{
		if (etage<heights.size())
			return heights[etage];
		return -1;
	}
	
	int Colib::getCenterOfColumnX(bool back) const
	{
		int x = 0;
		if (!back)
			x = 2*(Column::DEPTH + Cloison::THICKNESS);
		return x+Column::DEPTH/2;
	}

	
	int Colib::getCenterOfColumnZ(unsigned int col_nr) const
	{
		int z = Bati::THICKNESS;
		if (col_nr<columns_back.size())	// TODO columns may have different widths by design (not physically)
		{
			int last_z;
			for(auto column: columns_back)
			{
				last_z = z;
				z += column->getWidth() + Cloison::THICKNESS;
				if (col_nr==0)
					break;
				col_nr--;
			}
			return (last_z + z)/2 + Cloison::THICKNESS/2;
		}
		return 0;
	}

	
	void Colib::recalcSize() {
		int sizes = 2;
		// length = colCount*COL_WIDTH+(colCount-1)*Cloison::THICKNESS;
		
		int length_back = computeLength(columns_back);
		int length_front = computeLength(columns_front);
		
		if (length_front)
			sizes=3;
		
		length = 2*Bati::THICKNESS + (length_back > length_front ? length_back : length_front);
		width =  sizes * Column::DEPTH+(sizes-1)*Cloison::THICKNESS;
		
		heights.clear();
		// FIXME plateaux.clear()
		
		int h=Navette::HEIGHT + Bati::THICKNESS;
		last_frozen=0;
		while(h<getHeight())
		{
			heights.push_back(h);
			h += Column::ALVEOLE_MIN_HEIGHT;
			if (h < Column::ALVEOLE_SPLIT_FROST*getHeight()/100)
				last_frozen++;
		}
	}
}


