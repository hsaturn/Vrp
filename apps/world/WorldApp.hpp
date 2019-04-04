/* 
 * File:   World.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */

#ifndef WORLD_HPP
#define	WORLD_HPP

#include <Object.h>
#include <ObjectBuilder.hpp>
#include "Blocks/Block.hpp"
#include <Coord.hpp>
#include <string>
#include <map>

using namespace std;

namespace World {

	class Map;
	class WorldApp : public Application {
		
		APPLICATION_BUILDER("world", WorldApp);

	public:
		WorldApp(const string &name, string& incoming);

		~WorldApp() override;

		bool _render(bool resetTimer);
		ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) override;
		
	private:

		Map* m_map;
	};
}
#endif	/* WORLD_HPP */

