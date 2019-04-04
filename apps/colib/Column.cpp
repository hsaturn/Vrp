#include "Column.hpp"
#include "ColibApp.hpp"

class Model;
namespace Colib
{
	Column::Column(ColibApp* colib, int width)
		:
		mwidth_z(width),
		pcolib(colib)
	{
		
	}
	
	Column::Column(ColibApp* colib, istream& in, int center_x)
		: pcolib(colib)
	{
		string s;
		in >> s;
		if (s == "width")
			in >> mwidth_z;
		else
		{
			mwidth_z = 0;
			return;
		}
		in >> s;
		if (s=="{")
		{
			while(in.good() && s!="}")
			{
				int num_cell;
				string content;
				
				in >> s;
				if (s=="}")
					break;
				num_cell=atoi(s.c_str());
				in >> content;
				
				Plateau* p = Plateau::factory(content, center_x);
				if (p)
				{
					if (alveoles.find(num_cell)==alveoles.end())
						alveoles[num_cell] = p;
					else
					{
						delete p;
						cerr << "Column::restore, Conflict num_cell for Plateau (" << num_cell << ' ' << content << ')' << endl;
					}
				}
			}
			if (s != "}")
				cerr << "Column::restore, end of column not found '}' in input file." << endl;
		}
		else
			cerr << "Column::restore, Expecting { in input for column description, got '" << s << "' instead." << endl;
	}
	
	Column::~Column()
	{
		for(const auto& plateau: alveoles)
			if (plateau.second)
				delete plateau.second;
	}
	
	bool Column::save(const string name, const vector<Column*>& cols, ostream &out)
	{
		out << name <<  " {" << endl;
		for(const auto& col : cols)
			col->save(out);
		out << "}" << endl;
		return true;
	}
	
	bool Column::save(ostream& out) const
	{
		out << "  width " << mwidth_z << " { ";
		for(const auto& [num_cellule, plateau] : alveoles)
			if (plateau)
				out << num_cellule << " " << plateau->getContent() << ' ';
		out << '}' << endl;
		return true;
	}
	
	bool Column::restore(ColibApp* pcolib, vector<Column*>& cols, istream& in, int center_x)
	{
		for(const auto& colPtr: cols)
			delete colPtr;
      
		cols.clear();
		
		string s;
		in >> s;
		if (s != "{")
		{
			cerr << "Missing { for column." << endl;
			return false;
		}
		while(in.good() && s!="}")
		{
			Column* column = new Column(pcolib, in, center_x);
			if (column->getWidth())
				cols.push_back(column);
			else
			{
				delete column;
				break;
			}
		}
		return true;
	}

	
	bool Column::hasRoomFor(int etage, const Plateau* plateau) const
	{
		if (getPlateau(etage))
			return false;
		
		if (plateau->getWidth() != getWidth())
		{
			cerr << "Largeurs incompatibles." << endl;
			return false;
		}

		int h1 = pcolib->getHeight(etage);
		int h2 = h1+plateau->getHeight();
		cout << "-------------" << endl;
		cout << "from " << h1 << " to " << h2 << endl;

		if (pcolib->collideVertical(h1, h2, false))		// @FIXME, false ??? we should use back flag
			return false;
		
		for(const auto& it: alveoles)
		{
			Plateau* plateau=it.second;
			if (plateau)
			{
				bool collide = false;
				int h = pcolib->getHeight(it.first);
				int hp = h + plateau->getHeight();
				
				if (h >= h1 && h<=h2)
					collide = true;
				if (hp >= h1 && h<=h2)
					collide = true;
				if (h1>>h && h2<=hp)
					collide = true;
				if (collide)
				{
					cerr << "Collision avec le plateau de l'étage " << it.first << ", " << plateau->getContent() << endl;
					return false;
				}
				cout << "Pas de collision avec " << plateau->getContent() << " " << pcolib->getHeight(it.first) << " to " << h << endl;
			}
		}		
		
		if (h1+plateau->getHeight() > pcolib->getHeight())
		{
			cerr << "Ca touche le plafond (cela dit, c'est impossible à cet endroit du code !" << endl;
			return false;
		}
		
		return true;
	}
	
	Plateau* Column::getPlateau(int etage) const
	{
		map<int, Plateau*>::const_iterator it=alveoles.find(etage);
		if (it!=alveoles.end())
			return it->second;
		return 0;
	}
	
	void Column::setPlateau(int etage, Plateau* p)
	{
		alveoles[etage] = p;
	}

	void Column::render(int x1, int x2, int z)
	{
		for(const auto& [cell, plateau]: alveoles)
			if (plateau)
				plateau->renderAtCenter(pcolib->getHeight(cell), z);
	}

}