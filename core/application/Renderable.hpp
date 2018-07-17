/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Renderable.hpp
 * Author: hsaturn
 *
 * Created on 17 août 2017, 08:27
 */

#ifndef RENDERABLE_HPP
#    define RENDERABLE_HPP

typedef list<string> CmdQueue;

class Renderable
{
  public:
	typedef enum ExecResult
	{
		EXEC_UNKNOWN, // Unknown (return this by default)
		EXEC_FAILED, // Known, execution failed
		EXEC_OK, // Execution ok
		EXEC_BUSY // Known but cannot run now
	} ExecResult;

  public:
	Renderable() {};
	virtual ~Renderable() {};

	virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) = 0;

	virtual void _help(Help&) { };

	virtual void _onVarChanged(const string& name, const string& value) { };

	/**
	 * @return true if need to redisplay asap
	 */
	virtual bool _render(bool resetTimer) = 0;

  private:
	Renderable(const Renderable& orig);
};

#endif /* RENDERABLE_HPP */

