/* 
 * File:   RedMap.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#ifndef REDMAP_HPP
#define	REDMAP_HPP

#include <Object.h>
#include <ObjectBuilder.hpp>
#include "Blocks/Block.hpp"
#include <Coord.hpp>
#include <string>
#include <map>

using namespace std;

namespace RedStone {

	class RedMap : public Application {

		APPLICATION_BUILDER("redmap", RedMap);

	  public:
		RedMap(const string &name, string& incoming);

		virtual ~RedMap() ;

		virtual bool _render(bool resetTimer);
		virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
		virtual void _help(Help&);
		
		/** return 0 or a Block */
		Block* getTile(const Coord&) const;

	private:

		static RedMapBuilder builder;
		map<Coord, Block*>	tiles;
		map<Coord, Block*> ground;
	};
}
#endif	/* REDMAP_HPP */

