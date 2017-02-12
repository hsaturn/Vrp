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
	int col=0;
	int vert=0;
	bool back=true;
		
	Colib::ColibBuilder Colib::builder;
	
	Colib::Colib(const string& name, string &incoming)
	: Object(name)
	{
		list_columns = 0;
		height = StringUtil::getFloat(incoming);

		if (height<100)
			height = 100;
		if (height>500)
			height = 500;
		
		sizeHasChanged();
		
		setVar("scale", "0.1");
		
		bati = new Bati(this);
		setVar("dx" ,StringUtil::to_string(-width/20));
		setVar("dz", StringUtil::to_string(-getLength()/20));
	}
	
	Colib::~Colib()
	{
		cout << "DELETING COLIB" << endl;
		delete bati;
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
		
		if (list_columns == 0)
		{
			list_columns = glGenLists(1);
			if (list_columns)
				glNewList(list_columns, GL_COMPILE);
			
			// Rendu des cloisons uniquement
			renderColumns(columns_back, 0, true);
			renderColumns(columns_front, 2*(Column::DEPTH_X + Cloison::THICKNESS_Z), true);
			
			glEndList();
		}
		if (list_columns)
			glCallList(list_columns);
		
		// Pour les plateaux uniquement
		renderColumns(columns_back, 0, false);
		renderColumns(columns_front, 2*(Column::DEPTH_X + Cloison::THICKNESS_Z), false);
		
		return bati->render();
	}
	
	void Colib::renderColumns(vector<Column*>& columns, int x1, bool bCloisons)
	{
		if (columns.size())
		{
			int x2 = x1 + Column::DEPTH_X;
			int z=Bati::THICKNESS - Cloison::THICKNESS_Z + Cloison::HOLLOW_Z;
			
			if (bCloisons)
			{
				// Cloison, Jupes et tablier
				Cloison::render(x1, x2, getHeight(), z, heights);
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

			for(Column* column: columns)
			{
				z += column->getWidth()/2 + Cloison::THICKNESS_Z-Cloison::HOLLOW_Z;
				if (!bCloisons)
					column->render(x1,x2,z);
				
				z += column->getWidth()/2  - Cloison::HOLLOW_Z;
				
				if (bCloisons)
					Cloison::render(x1, x2, getHeight(), z, heights);
			}
			
			if (bCloisons && last_frozen)
			{
				Color::light_gray.render();
				glBegin(GL_TRIANGLE_STRIP);
				float h = getHeight(last_frozen)-0.01;
				glVertex3f(x1, h, 0);
				glVertex3f(x2, h, 0);
				glVertex3f(x1, h, length);
				glVertex3f(x2, h, length);
				glEnd();
			}
		}
	}

	
	bool Colib::_execute(Server* server, string cmd, string incoming, const string& org, CmdQueue&)
	{
		string error="";
		bool bRet = true;
		
		if (cmd=="go" || cmd=="next" || cmd=="prev")
		{
			int revcol = col;
			int revvert= vert;
			
			if (cmd=="next" || cmd=="prev")
			{
				int dir=1;
				string what = StringUtil::getWord(incoming);

				if (cmd=="prev") dir=-1;
				
				if (what=="col")
					col += dir;
				else if (what=="cell")
					vert += dir;
				else if (what=="side")
					back = !back;
				else
				{
					cerr << "Invalid : " << what << endl;
				}
			}
			else
			{
				col = StringUtil::getLong(incoming);	// Colonne
				vert = StringUtil::getLong(incoming);	// Cellule verticale
				
				if (incoming.substr(0,5)=="front")
				{
					back=false;
					StringUtil::getWord(incoming);
				}
				else
					back = true;
			}
			
			if (getHeight(vert)==-1)
			{
				cout << "ERREUR cellule" << endl;
				vert=revvert;
				return true;
			}
			
			if (getCenterOfColumnZ(col, back)==-1)
			{
				cout << "ERREUR column";
				col=revcol;
				return true;
			}
			
			stringstream out;
			if (bati->moveTo(col,vert,back))
				out << "MOVE TO " << col << 'x' << vert << endl;
			else
			{
				col = revcol;
				vert = revvert;
				out << "CANNOT MOVE THERE" << endl;
			}
			cout << out.str();
			server->send(out.str());
		}
		else if (cmd=="height")
		{
			height = StringUtil::getLong(incoming);
			if (height<100)
				height=100;
			else if (height>2000)
				height = 2000;
			sizeHasChanged();
		}
		else if (cmd=="sound")
		{
			bati->changeSound(incoming);
		}
		else if (cmd=="load")
		{
			if (bati->isReady())
			{
				if (bati->getPlateau())
					error="Not empty !";
				else
				{
					Plateau* plateau = Plateau::factory(incoming, (bati->getXLeft()+bati->getXRight())/2);
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
				else
					error = "Invalid (back or front expected)";
				if (pcols)
				{
					int width = StringUtil::getLong(incoming);
					
					if (width<20 || width>100)
						error = "Invalid width (from 20 to 200)";
					else
					{
						Column* column = new Column(this, width);
						pcols->push_back(column);
						sizeHasChanged();
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
			const char* err= bati->put(back);
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
			const char* err=bati->get(back);
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
		help.add("next/prev col/cell/side");
		help.add("put");
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
		
		int ep = Cloison::THICKNESS_Z - 2*Cloison::HOLLOW_Z;
		
		if (columns.size() > 1)
			length += ep * (columns.size()-1);
				
		return length;
	}
	
	int Colib::getHeight(unsigned int etage) const
	{
		if (etage<heights.size())
			return heights[etage];
		return -1;
	}
	
	float Colib::getCenterOfColumnX(bool back) const
	{
		float x = 0;
		if (!back)
			x = 2*(Column::DEPTH_X + Cloison::THICKNESS_Z);
		return x+Column::DEPTH_X/2.0;
	}

	
	float Colib::getCenterOfColumnZ(unsigned int col_nr, bool back) const
	{
		const vector<Column*>* pcolumns;
		
		if (back)
			pcolumns = &columns_back;
		else
			pcolumns = &columns_front;
		
		if (col_nr >= pcolumns->size())
		{
			cerr << "Column out of range (max is " << pcolumns->size() << ')' << endl;
			return -1;
		}

		float z = Bati::THICKNESS;
		for(auto column: *pcolumns)
		{
			if (col_nr==0)
			{
				z+= column->getWidth()/2;
				break;
			}
			else
				z += column->getWidth() + Cloison::THICKNESS_Z - 2*Cloison::HOLLOW_Z;
			col_nr--;
		}
		return z;
	}

	
	void Colib::sizeHasChanged() {
		int widths = 2;
		// length = colCount*COL_WIDTH+(colCount-1)*Cloison::THICKNESS;
		
		// Calcul distance entre bati (plus proches faces)
		
		int length_back = computeLength(columns_back);
		int length_front = computeLength(columns_front);
		
		if (length_front)
			widths=3;
		
		length = 2*Bati::THICKNESS + (length_back > length_front ? length_back : length_front);
		width =  widths * Column::DEPTH_X+(widths-1)*Cloison::THICKNESS_Z;
		
		heights.clear();
		// FIXME plateaux.clear()
		
		int h=Navette::HEIGHT_Y + Bati::THICKNESS;
		last_frozen=0;
		while(h<getHeight())
		{
			heights.push_back(h);
			h += Column::ALVEOLE_MIN_HEIGHT;
			if (h < Column::ALVEOLE_SPLIT_FROST*getHeight()/100)
				last_frozen++;
		}
		if (list_columns)
		{
			glDeleteLists(list_columns, 1);
			list_columns = 0;
		}
	}
	
	const char* Colib::collideVertical(int h1, int h2, bool back) const
	{
		const char* reason = 0;
		//  @todo /back & forbidden area (guichet)
		if (h1>h2)
		{
			int h = h1;
			h1 = h2;
			h2 = h;
		}
		if (h1<Navette::HEIGHT_Y + Bati::THICKNESS)
			reason = "Too low";
		else if (h2>=getHeight())
			reason = "Too high";
		else if (last_frozen)
		{
			int frozen_h = getHeight(last_frozen);
			if (h1<frozen_h && h2>=frozen_h)
				reason = "Collide frozen zone";
		}
		if (reason)
			cerr << reason << endl;
		return reason;
	}
}


