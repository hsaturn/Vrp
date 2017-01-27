/* 
 * File:   RedStone.hpp
 * Author: hsaturn
 *
 * Created on 5 avril 2016, 20:36
 */

#ifndef REDSTONE_HPP
#define	REDSTONE_HPP

#include "Object.h"
#include <string>

using namespace std;

namespace RedStone
{
	
class RedStone : public Object {
	class RedStoneBuilder : public ObjectBuilder {
	public:
		RedStoneBuilder() : ObjectBuilder("redstone") {
		}
		
		virtual ~RedStoneBuilder() {}
		
		Object* build(const string& name, string& incoming) {
			return new RedStone(name, incoming);
		}
		
	};
public:
	RedStone(const string& name, string& incoming);
	virtual ~RedStone(){}
	
	virtual bool _render(bool resetTimer);
	virtual bool _execute(Server*, string cmd, string incoming, const string& org);
	virtual void _help(Help&);
	
private:
	RedStone(const RedStone& orig);
	
	static RedStoneBuilder builder;

};

}

#endif	/* REDSTONE_HPP */

