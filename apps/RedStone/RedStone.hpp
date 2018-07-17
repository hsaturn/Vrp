/* 
 * File:   RedStone.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:36
 */

#ifndef REDSTONE_HPP
#    define REDSTONE_HPP

#    include <Object.h>
#    include <ObjectBuilder.hpp>
#    include <string>

using namespace std;

namespace RedStone
{

	class RedStone : public Application
	{
		APPLICATION_BUILDER("redstone", RedStone);

	  public:
		RedStone(const string& name, string& incoming);

		~RedStone() override { }

		bool _render(bool resetTimer) override;
		ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
		void _help(Help&) override;

	  private:
		RedStone(const RedStone& orig);

		static RedStoneBuilder builder;

	};

}

#endif /* REDSTONE_HPP */

