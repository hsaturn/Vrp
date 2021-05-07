/* 
 * File:   RedMap.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:47
 */
#pragma once

#include <Application.h>
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

		~RedMap() override;

		bool _render(bool resetTimer) override;
		ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) const override;
		
		/** return 0 or a Block */
		Block* getTile(const Coord&) const;

	private:
		map<Coord, Block*> tiles;
		map<Coord, Block*> ground;
	};
}
