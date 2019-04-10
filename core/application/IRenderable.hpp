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

#pragma once

using CmdQueue = list<string>;

class IRenderable
{
  public:
	IRenderable() = default;
	virtual ~IRenderable() = default;

	virtual void _onVarChanged(const string& name, const string& value) { };   // TODO refactor in Application or IRunnable ?

	/**
	 * @return true if need to redisplay asap
	 */
	virtual bool _render(bool resetTimer) = 0;

  private:
	IRenderable(const IRenderable& orig);
};
