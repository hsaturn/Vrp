/* 
 * File:   Colib.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#ifndef COLIB_HPP
#define	COLIB_HPP

#include <Object.h>
#include "Coord.hpp"
#include <string>
#include <map>

using namespace std;

namespace Colib {

	class Bati;
	
	class Colib : public Object {

		class ColibBuilder : public ObjectBuilder {
		public:

			ColibBuilder() : ObjectBuilder("colib") {
			}

			virtual ~ColibBuilder() {
			}

			Object* build(const string& name, string& incoming) {
				return new Colib(name, incoming);
			}
		};
		
	public:
		static const int COL_WIDTH=60;	// Largeur utile d'une colonne en cm
		static const int CLO_THICK=2;	// Epaisseur cloison en cm
		static const int CFG_WIDTH=60;	// Largeur d'une rangée
		
		Colib(const string &name, string& incoming);

		virtual ~Colib() ;

		virtual void _render(bool resetTimer);
		virtual bool _execute(Server*, string cmd, string incoming, const string& org);
		virtual void _help(Help&);
		
		/**
		 * @return width 2 or 3
		 */
		int getWidth() const { return width; }
		
		/**
		 * @return length : Number of columns
		 */
		int getColumnsCount() const { return colCount; }
		
		/**
		 * @return height in cm
		 */
		int getHeight() const { return height; }
		
		int getLength() const { return length; }
		
	private:
		
		void recalcSize();

		static ColibBuilder builder;
		
		int config;		// 2 ou 3 (1 ou 2 cotés + espace central)
		int colCount;	// longueur en nombre de colonnes
		int height;		// Hauteur en cm
		
		int width;		// Largeur en cm
		int length;		// Longeur en cm
		
		Bati* bati;
	};
}
#endif	/* COLIB_HPP */

