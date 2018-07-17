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

		virtual ~RedStone() { }

		virtual bool _render(bool resetTimer);
		virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
		virtual void _help(Help&);

	  private:
		RedStone(const RedStone& orig);

		static RedStoneBuilder builder;

	};

}

#endif /* REDSTONE_HPP */

