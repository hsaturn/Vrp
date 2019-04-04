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
#include <vector>
#include <map>
#include "Plateau.hpp"

using namespace std;

namespace Colib
{
	class ColibApp;
	
	class Column
	{
	public:
		static const int ALVEOLE_MIN_HEIGHT = 5;
		static const int ALVEOLE_SPLIT_FROST = 40;	// en %
		static const int DEPTH_X=60;	// Profondeur d'une colonne en cm
		
		Column(ColibApp*, int width);
		Column(ColibApp*, istream&, int center_x);
		~Column();
		
		// Rendu des plateaux uniquement
		void render(int x1, int x2, int z);
			
		int getWidth() const { return mwidth_z; }
		int getAlveoleCount() const;
		
		bool hasRoomFor(int etage, const Plateau*) const;
		Plateau* getPlateau(int etage) const;
		void setPlateau(int etage, Plateau* plateau);
		
		bool isEmpty(int alveole) const;
		
		void insertPlateau(shared_ptr<Plateau>, int alveole);
		
		bool save(ostream&) const;
		bool restore(ifstream& in, ColibApp* pcolib) const;
		
		static bool save(const string name, const vector<Column*>&, ostream& out);
		static bool restore(ColibApp* pcolib, vector<Column*>&, istream& in, int center_x);
			
	private:
		int mwidth_z;
		ColibApp* pcolib;
		map<int, Plateau*> alveoles;	// Map num cellule / plateau
	};
}


#endif /* COLUMN_HPP */

