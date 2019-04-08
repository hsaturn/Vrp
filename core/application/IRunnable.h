/* 
 * File:   IRunnable.h / IRUNNABLE_H
 * Author: hsaturn
 *
 * Created on 8 avril 2019, 22:00
 */

#pragma once

class IRunnable
{
public:
   IRunnable() = default;
   ~IRunnable() = default;
	typedef enum ExecResult
	{
		EXEC_UNKNOWN, // Unknown (return this by default)
		EXEC_FAILED, // Known, execution failed
		EXEC_OK, // Execution ok
		EXEC_BUSY // Known but cannot run now
	} ExecResult;
   
	virtual ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) = 0;

	virtual void _help(Help&) = 0;
};