/* 
 * File:   Colib.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */
#pragma once

#include <ObjectBuilder.hpp>
#include "Coord.hpp"
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace Colib {
	
	extern map<const char*, string> last;

	template<class T>
	void colog(const char* key, T data)
	{
		string s =StringUtil::to_string(data);
		if (last[key]==s)
			return;
		last[key] = s;
		cout << '[' << key << "] " << s << endl;
	}
	
	class Bati;
	class Column;
	
	class ColibApp : public Application {

		class ColibBuilder : public ApplicationBuilder {
		public:

			ColibBuilder() : ApplicationBuilder("colib") {};
			
			~ColibBuilder() override {}

			Application* build(const string& name, string& incoming);
		};
		
		void _help(Help&) override;
		
		bool save(string file);
		bool restore(string file);
		void center();
		
	public:
		static const int CLO_THICK=2;	// Epaisseur cloison en cm
		
		ColibApp(const string &name, int height);

		~ColibApp() override;

		bool _render(bool resetTimer) override;
		ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
		
		Column* getColumn(unsigned int nr, bool back=true) const;
		/**
		 * @return width 2 or 3
		 */
		int getWidth() const { return width_x; }
		
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
		
		float getLength() const { return length_z; }
		
		/**
		 * 
		 * @param col_nr
		 * @return center of column (z axis), or -1 (out of range)
		 */
		float getCenterOfColumnZ(unsigned int col_nr, bool back) const;
		float getCenterOfColumnX(bool back) const;
		
		static bool renderBoundingBoxes() { return gRenderBoundingBoxes; }
		
	private:
		
		void autosave();
		
		void sizeHasChanged();
		void renderColumns(vector<Column*>&, int x1, bool bCloisons);

		static ColibBuilder builder;
		static bool			gRenderBoundingBoxes;
		
		int height;		// Hauteur en cm
		
		int width_x;		// Largeur en cm
		int length_z;		// Longeur en cm
		
		Bati* bati;
		vector<Column*> columns_back;
		vector<Column*> columns_front;
		
		vector<int> heights;		// Hauteurs des plateaux
		int last_frozen;			// Dernier plateau froid (non inclu)
		GLuint list_columns;		// Indexe liste cloisons des colonnes ou 0
		bool last_ready;			// pour autosave
	};
}
