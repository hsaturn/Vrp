/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Column.hpp
 * Author: francois
 *
 * Created on 2 février 2017, 11:59
 */

#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <memory>
#include <map>
#include "Plateau.hpp"

using namespace std;

namespace Colib
{
	class Colib;
	
	class Column
	{
	public:
		static const int ALVEOLE_MIN_HEIGHT = 30;
		static const int ALVEOLE_MAX_HEIGHT = 60;
		static const int ALVEOLE_SPLIT_FROST = 40;	// en %
		static const int DEPTH=60;	// Profondeur d'une colonne en cm
		
		Column(Colib*, int width);
		
		void render(int x1, int x2, int z);
			
		int getWidth() const { return mwidth; }
		int getAlveoleCount() const;
		
		Plateau* getPlateau(int etage) const;
		void setPlateau(int etage, Plateau* plateau);
		
		bool isEmpty(int alveole) const;
		
		void insertPlateau(shared_ptr<Plateau>, int alveole);
			
	private:
		int mwidth;
		map<int, Plateau*> alveoles;	// Map num cellule / plateau
		Colib* pcolib;
	};
}


#endif /* COLUMN_HPP */

