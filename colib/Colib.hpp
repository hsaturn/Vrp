/* 
 * File:   Colib.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#ifndef COLIB_HPP
#define	COLIB_HPP

#include <Object.h>
#include <ObjectBuilder.hpp>
#include <GL/glew.h>
#include "Coord.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace Colib {

	class Bati;
	class Column;
	
	class Colib : public Object {

		class ColibBuilder : public ObjectBuilder {
		public:

			ColibBuilder() : ObjectBuilder("colib") {}

			virtual ~ColibBuilder() {}

			Object* build(const string& name, string& incoming) {
				return new Colib(name, incoming);
			}
		};
		
		virtual void _help(Help&);
		
	public:
		static const int CLO_THICK=2;	// Epaisseur cloison en cm
		
		Colib(const string &name, string& incoming);

		virtual ~Colib() ;

		virtual bool _render(bool resetTimer);
		virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
		
		Column* getColumn(unsigned int nr, bool back=true) const;
		/**
		 * @return width 2 or 3
		 */
		int getWidth() const { return width; }
		
		/**
		 * @return height in cm
		 */
		int getHeight() const { return height; };
		
		/**
		 * @param h1
		 * @param h2
		 * @return not 0 if h1/h2 collides with colib 'walls'
		 * (frozen horizontal barrier, roof etc...)
		 */
		const char* collideVertical(int h1, int h2, bool back) const;
		
		/**
		 * 
		 * @param etage numéro de l'étage à atteindre
		 * @return hauteur (basse) de l'étage, ou -1 (out of range)
		 */
		int getHeight(unsigned int etage) const;
		
		int getLength() const { return length; }
		
		/**
		 * 
		 * @param col_nr
		 * @return center of column (z axis), or -1 (out of range)
		 */
		float getCenterOfColumnZ(unsigned int col_nr, bool back) const;
		float getCenterOfColumnX(bool back) const;
		
	private:
		
		void sizeHasChanged();
		void renderColumns(vector<Column*>&, int x1, bool bCloisons);

		static ColibBuilder builder;
		
		int config;		// 2 ou 3 (1 ou 2 cotés + espace central)
		int colCount;	// longueur en nombre de colonnes
		int height;		// Hauteur en cm
		
		int width;		// Largeur en cm
		int length;		// Longeur en cm
		
		Bati* bati;
		vector<Column*> columns_back;
		vector<Column*> columns_front;
		
		vector<int> heights;		// Hauteurs des plateaux
		int last_frozen;			// Dernier plateau froid (non inclu)
		GLuint list_columns;		// Indexe liste cloisons des colonnes ou 0
	};
}
#endif	/* COLIB_HPP */

