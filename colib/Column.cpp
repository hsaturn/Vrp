#include "Column.hpp"
#include "Colib.hpp"

namespace Colib
{

	Column::Column(Colib* colib, int width)
		:
		mwidth(width),
		pcolib(colib)
	{
		
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
				p->renderAtCenter(h, z+getWidth()/2);
			}
		}
	}

}