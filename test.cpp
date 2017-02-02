#include <iostream>
#include <list>

class BASE
{
public:
	virtual void dump()=0;
	
};

template<int from, int to>
class Z : public BASE
{
public:
	Z() { std::cout << "Z" << std::endl; }
	virtual void dump() { std::cout << "outZ(" << from << ',' << to << ')' << std::endl; }
};


class K : public BASE
{
public:
	K() { std::cout << "K" << std::endl; }
	virtual void dump() { std::cout << "outK" << std::endl; }
};

class X : public BASE
{
public:
	X() { std::cout << "X" << std::endl; }
	virtual void dump() { std::cout << "outX" << std::endl; }

};


template <typename... T>
struct tuple {
  tuple() {
	lst = { new T... };
  }
  

  void dump()
  {
	  for(auto a: lst)
		  a->dump();
  };
  
  std::list<BASE*> lst;
  //T... tail;
};

template <class T>
T* build()
{
	return new T;
}

int main(int argc, const char* argv[])
{
	tuple<
		Z<1,2>,
		X,
		Z<100, 200>
		> t_terminal;

	tuple<
		Z_NOTERM<1,2>,
		X,
		Z<100, 200>
		> t_not_terminal;
	
	t.dump();
	return 0;
}