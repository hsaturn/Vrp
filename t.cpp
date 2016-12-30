#include <iostream>
#include <map>

using namespace std;

typedef int8_t Int;
	
class Coord {
public:
	Coord(Int x, Int y, Int z) {
		c.key=0;
		c.coords[0]=x;
		c.coords[1]=y;
		c.coords[2]=z;		
	};

	Int x() const { return c.coords[0]; }
	Int y() const { return c.coords[1]; }
	Int z() const { return c.coords[2]; }
	uint32_t key() const { return c.key; }
	
	/* Assignable values (bad design but easy */
	Int& X() { return c.coords[0]; }
	Int& Y() { return c.coords[1]; }
	Int& Z() { return c.coords[2]; }
	
	bool operator < (const Coord& b) const
	{
		return c.key < b.c.key;
	}

	
	friend inline ostream& operator << (ostream& out, const Coord& c)
	{
		out << '(' << c.c.key << ", " << (int)c.x() << ", " << (int)c.y() << ", " << (int)c.z() << ", "<<  ')';
		return out;
	}
	
private:
	
	typedef union
	{
		Int coords[3];
		uint32_t key;
	} coord;
	
	coord c;
	
};

int main(int argc, const char* argv[])
{
	map<Coord, int>	m;

	int i=0;

	for(int x=0; x<20; x++)
	{
		Coord c(x,0,0);
		m[c]=i++;
	}

	for(auto it: m)
		cout << it.first << " : " << it.second << endl;

	return 0;
}
