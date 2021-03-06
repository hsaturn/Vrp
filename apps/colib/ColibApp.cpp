/*
 * File:   Colib.cpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#include <fstream>

#include "ColibApp.hpp"
#include "StringUtil.hpp"
#include "Bati.hpp"
#include "Navette.hpp"
#include "Column.hpp"
#include "Cloison.hpp"
#include "Decor.hpp"


namespace Colib
{
	map<const char*, string> last;

	int col=-1;
	int vert=0;
	bool back=true;

	ColibApp::ColibBuilder ColibApp::builder;
	bool ColibApp::gRenderBoundingBoxes=false;

	Application* ColibApp::ColibBuilder::build(const string& name, string& incoming)
	{
		ColibApp* pcolib;
		string sheight = StringUtil::getWord(incoming);
		if (StringUtil::is_integer(sheight))
		{
			 pcolib = new ColibApp(name, atoi(sheight.c_str()));
			 cout << sheight << " is int." << endl;
		}
		else
		{
			cout << "Reading colib from file " << sheight << endl;
			pcolib = new ColibApp(name, 200);
			if (!pcolib->restore(sheight))
			{
				delete pcolib;
				pcolib = 0;
				cerr << "Error while restoring Colib from " << sheight << " file." << endl;
			}
		}

		return pcolib;
	}

	ColibApp::ColibApp(const string& name, int initial_height)
	: Application(name), height(initial_height)
	{
		last_ready = false;
		list_columns = 0;

		if (height<100)
			height = 100;
		if (height>500)
			height = 500;

		sizeHasChanged();

		setVar("scale", "0.1");

		bati = new Bati(this);
	}

	ColibApp::~ColibApp()
	{
		cout << "DELETING COLIB" << endl;
		delete bati;
	}

	bool ColibApp::_render(bool resetTimer)
	{
		glPushMatrix();
		glTranslatef(width_x/2,0,length_z/2);
		Decor::render(30*((width_x+200)/30), 0, 30*((length_z+200)/30), 30);
		glPopMatrix();

		/*
		Model::get("pan")->render();
		glTranslatef(0, 0, -40);
		Model::get("cactus")->render();
		glTranslatef(0, 0, -40);
		Model::get("plant")->render();
		glTranslatef(0, 0, -40);
		Model::get("caisse")->render();
		return false;
		*/

		Color::dark_green.render();
		glBegin(GL_TRIANGLE_FAN);
		glNormal3i(0, 1, 0);
		glVertex3f(0,-0.01,0);
		glVertex3f(width_x,-0.01,0);
		glVertex3f(width_x, -0.01, length_z);
		glVertex3f(0, -0.01, length_z);
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

		bool brender = bati->_render(resetTimer);
		if (brender != last_ready)
		{
			last_ready = brender;
			autosave();
		}
		return brender;
	}

	void ColibApp::renderColumns(vector<Column*>& columns, int x1, bool bCloisons)
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
				glVertex3i(x1, 0, length_z);
				glVertex3i(x1, h, 0);
				glVertex3i(x1, h, length_z);
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);
				glVertex3i(x2, 0, 0);
				glVertex3i(x2, 0, length_z);
				glVertex3i(x2, h, 0);
				glVertex3i(x2, h, length_z);
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);

				float hf = h-0.01;
				glVertex3f(x1, hf, 0);
				glVertex3f(x2, hf, 0);
				glVertex3f(x1, hf, length_z);
				glVertex3f(x2, hf, length_z);
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
				glVertex3f(x1, h, length_z);
				glVertex3f(x2, h, length_z);
				glEnd();
			}
		}
	}

	IRunnable::ExecResult ColibApp::_execute(Server* server, string cmd, string incoming, const string& org, CmdQueue&)
	{
		string error="";
		ExecResult ret = EXEC_OK;

		if (cmd=="go" || cmd=="next" || cmd=="prev")
		{
			if (!bati->isReady())
				return EXEC_BUSY;
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
					cerr << "Invalid : " << what << endl;
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
				return EXEC_FAILED;
			}

			if (getCenterOfColumnZ(col, back)==-1)
			{
				cout << "ERREUR column";
				col=revcol;
				return EXEC_FAILED;
			}

			stringstream out;
			ret = bati->moveTo(col,vert,back);
			if (ret == EXEC_OK)
			{
				autosave();
				out << "MOVE TO " << col << 'x' << vert << endl;
			}
			else if (ret == EXEC_FAILED)
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
			autosave();
		}
		else if (cmd=="autosave")
		{
			string s = StringUtil::getWord(incoming);
			if (s=="on" || s=="off")
				setVar("autosave", s);
			else
				cerr << "autosave [on|off]" << endl;
		}
		else if (cmd=="autocenter")
		{
			string s = StringUtil::getWord(incoming);
			if (s=="on" || s=="off")
			{
				setVar("autocenter", s);
				if (s=="on") center();
			}
			else
				cerr << "autocenter [on|off]" << endl;
		}
		else if (cmd=="sound")
		{
			string income2(incoming);
			string sound_name=StringUtil::getWord(income2);
			if (sound_name=="bati" || sound_name=="navette")
			{
				if (incoming.length())
					setVar("sound_"+sound_name, incoming);
				else
					incoming=getString("sound_"+sound_name);
			}
			cout << "SOUND " << incoming << endl;
			bati->changeSound(incoming);
			// @TODO save sound
		}
		else if (cmd=="load")
		{
			if (bati->isAllStopped())
			{
				if (bati->getPlateau())
					error="Not empty !";
				else
				{
					Plateau* plateau = Plateau::factory(incoming, (bati->getXLeft()+bati->getXRight())/2);
					bati->setPlateau(plateau);
					autosave();	// @TODO save current plateau
				}
			}
			else
				return EXEC_BUSY;
		}
		else if (cmd=="add")
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
					autosave();
				}
			}
			else
				error="Missing back/front";
		}
		else if (cmd=="restore")
		{
			if (restore(incoming))
			{
				incoming=="";
				return EXEC_OK;
			}
			return EXEC_FAILED;
		}
		else if (cmd=="save")
		{
			if (bati->isReady())
			{
				error = save(incoming);
				incoming="";
				return EXEC_OK;
			}
			else
				return EXEC_BUSY;
		}
		else if (cmd=="center")
		{
			center();
			autosave();
			return EXEC_OK;
		}
		else if (cmd=="boundings")
		{
			if (incoming=="on")
			{
				gRenderBoundingBoxes = true;
				StringUtil::getWord(incoming);
			}
			else if (incoming=="off")
			{
				gRenderBoundingBoxes = false;
				StringUtil::getWord(incoming);
			}
			else if (StringUtil::is_integer(incoming))
				gRenderBoundingBoxes = StringUtil::getLong(incoming);
			else
			{
				error = "Bad parameter for boundings : " + incoming;
			}
		}
		else if (cmd=="put")
		{
			if (bati->isAllStopped())
			{
				if (getCenterOfColumnZ(col, back) != -1)
				{
					cout << "Colib.put " << endl;
					const char* err= bati->put(back);
					if (err)
						error=err;
					else
						autosave();
				}
				else
					error = "Position courante incorrecte";
			}
			else
				ret = IRunnable::EXEC_BUSY;
		}
		else if (cmd=="drop" || cmd=="remove")
		{
			bati->remove();
			autosave();
		}
		else if (cmd=="get")
		{
			if (!bati->isAllStopped())
				ret = EXEC_BUSY;
			else
			{
				StringUtil::trim(incoming);
				const char* err=bati->get(back);
				if (err)
					error=err;
				else
					autosave();
			}
		}
		else
			ret = EXEC_UNKNOWN;

		if (error.length())
		{
			server->send("ERROR : " + error);
			cerr << "ERROR : " << error << endl;
			cout << "ERROR : " << error << endl;
			return EXEC_FAILED;
		}
		return ret;
	}

	void ColibApp::center()
	{
		setVar("dx" , -width_x/20);
		setVar("dz", -length_z/20);
		cout << "dx=" << -width_x/20 << ", dz=" << -length_z/20 << endl;
	}

	void ColibApp::autosave()
	{
		if (getString("autosave")=="on")
			save(getString("last_save"));
	}

	bool ColibApp::save(string file)
	{
		if (file.length())
		{
			file="data/apps/colib/"+file;
			setVar("last_save", file);
			setVar("bouding_boxes", gRenderBoundingBoxes);
			ofstream output(file.c_str());
			if (output.is_open())
			{
				saveVars(output);
				output << "height " << height << endl;
				Column::save("back", columns_back, output);
				Column::save("front", columns_front, output);
				output << "END" << endl;
				return true;
			}
		}
		return false;
	}

	bool ColibApp::restore(string file)
	{
		try
		{
			file="data/apps/colib/"+file;
			ifstream input(file.c_str());
			if (!input.is_open())
				throw "Cannot open file "+file;
			cout <<"restoring from "+file << endl;
			loadVars(input);
			while(input.good())
			{
				string item;
				input >> item;
				cout << "ITEM " << item << endl;
				if (item=="end")
					break;
				else if (item=="height")
					input >> height;
				else if (item=="back")
					Column::restore(this, columns_back, input, getCenterOfColumnX(true));
				else if (item=="front")
					Column::restore(this, columns_front, input, getCenterOfColumnX(false));
			}
			gRenderBoundingBoxes = getFloat("bouding_boxes");
			string sound;
			sound = getString("bati");
			if (sound.length())
				bati->changeSound(sound);
			sound = getString("navette");
			if (sound.length())
				bati->changeSound(sound);
			sizeHasChanged();
		}
		catch(exception &e)
		{
			cerr << "Caught exception while restoring " << e.what() << endl;
		}
		catch(const char* p)
		{
			cerr << "ERROR while restoring : " << p << endl;
		}
		catch(const string& s)
		{
			cerr << "ERROR while restoring : " << s << endl;
		}
		catch(...)
		{
			cerr << "Caught unknown exception while restoring" << endl;
		}
		return true;
	}


	void ColibApp::_help(Help& help) const
	{
		help.add("go column, height%");
		help.add("next/prev col/cell/side");
		help.add("put/get");
		help.add("autosave [on|off]");
		help.add("center");
		help.add("save file");
		help.add("restore file");
		help.add("height n");
		help.add("drop|remove");
		help.add("sound");
		help.add("add [back|front] size");
	}

	Column* ColibApp::getColumn(unsigned int nr, bool back) const
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

	int ColibApp::getHeight(unsigned int etage) const
	{
		if (etage<heights.size())
			return heights[etage];
		return -1;
	}

	float ColibApp::getCenterOfColumnX(bool back) const
	{
		float x = 0;
		if (!back)
			x = 2*(Column::DEPTH_X + Cloison::THICKNESS_Z);
		return x+Column::DEPTH_X/2.0;
	}


	float ColibApp::getCenterOfColumnZ(unsigned int col_nr, bool back) const
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
		for(const auto& column: *pcolumns)
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


	void ColibApp::sizeHasChanged() {
		int widths = 2;
		// length = colCount*COL_WIDTH+(colCount-1)*Cloison::THICKNESS;

		// Calcul distance entre bati (plus proches faces)

		int length_back = computeLength(columns_back);
		int length_front = computeLength(columns_front);

		if (length_front)
			widths=3;

		length_z = 2*Bati::THICKNESS + (length_back > length_front ? length_back : length_front);
		width_x =  widths * Column::DEPTH_X+(widths-1)*Cloison::THICKNESS_Z;
		cout << "New length_z : " << length_z << endl;
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
		if (getString("autocenter")=="on")
			center();
	}

	const char* ColibApp::collideVertical(int h1, int h2, bool back) const
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


