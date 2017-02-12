#include "Column.hpp"
#include "Colib.hpp"

class Model;
namespace Colib
{
	Column::Column(Colib* colib, int width)
		:
		mwidth(width),
		pcolib(colib)
	{
		
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
		
		for(auto it: alveoles)
		{
			Plateau* p=it.second;
			if (p)
			{
				bool collide = false;
				int h = pcolib->getHeight(it.first);
				int hp = h + p->getHeight();
				
				if (h >= h1 && h<=h2)
					collide = true;
				if (hp >= h1 && h<=h2)
					collide = true;
				if (h1>>h && h2<=hp)
					collide = true;
				if (collide)
				{
					cerr << "Collision avec le plateau de l'étage " << it.first << ", " << p->getContent() << endl;
					return false;
				}
				cout << "Pas de collision avec " << p->getContent() << " " << pcolib->getHeight(it.first) << " to " << h << endl;
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
		for(auto it: alveoles)
		{
			Plateau* p=it.second;
			if (p)
			{
				int h = pcolib->getHeight(it.first);
				p->renderAtCenter(h, z);
			}
		}
	}

}