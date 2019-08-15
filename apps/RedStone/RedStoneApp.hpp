/* 
 * File:   RedStone.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:36
 */
#pragma once


#include <ObjectBuilder.hpp>
#include <string>

using namespace std;

namespace RedStone
{

	class RedStoneApp : public Application
	{
		APPLICATION_BUILDER("redstone", RedStoneApp);

	  public:
		RedStoneApp(const string& name, string& incoming);

		~RedStoneApp() override { }

		bool _render(bool resetTimer) override;
		ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) override;

	  private:
		RedStoneApp(const RedStoneApp& orig);

	};

}

