/* 
 * File:   Cloison.hpp
 * Author: francois
 *
 * Created on 2 février 2017, 14:49
 */

#ifndef CLOISON_HPP
#define CLOISON_HPP

#include <vector>
using namespace std;

namespace Colib
{
	class Cloison
	{
	public:
		static const int THICKNESS_Z = 3;
		static const int HOLLOW_Z = 1;		// profondeur rainure
		
		static void render(int x1, int x2, int height, int z, const vector<int>& heights);
	};
}



#endif /* CLOISON_HPP */

