/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SliceModel.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:06
 */
#pragma once

#include "core/Application.h"
#include "model/Model.hpp"
#include "glm/glm.hpp"
#include "application/IRunnable.h"
#include "application/IRenderable.hpp"

class Model;

namespace slicer
{
	
class SliceModel: public IRunnable, IRenderable
{
  public:
	SliceModel(const Model*);
	SliceModel(const SliceModel& orig)  = delete;
	virtual ~SliceModel() {};
	
	IRunnable::ExecResult load(string& incoming);
	
	bool _render(bool resetTimer) override;
   
   IRunnable::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
   void _help(Help&) const override;
	
	string file() const;
	
	const Model* getModel() const { return pmodel; }
	
  private:
	glm::vec3 translate;
	glm::vec3 rotate;
	const Model* pmodel;
};

}
