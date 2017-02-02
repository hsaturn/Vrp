/* 
 * File:   World.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include <Object.h>
#include "Blocks/Block.hpp"
#include <Coord.hpp>
#include <string>
#include <map>

using namespace std;

namespace World {

	class Map;
	class World : public Object {

		class WorldBuilder : public ObjectBuilder {
		public:

			WorldBuilder() : ObjectBuilder("world") {}

			virtual ~WorldBuilder() {}

			Object* build(const string& name, string& incoming) {
				return new World(name, incoming);
			}
		};
	public:
		World(const string &name, string& incoming);

		virtual ~World() ;

		virtual bool _render(bool resetTimer);
		virtual bool _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
		virtual void _help(Help&);
		
	private:

		static WorldBuilder builder;
		Map* m_map;
	};
}
#endif	/* WORLD_HPP */

