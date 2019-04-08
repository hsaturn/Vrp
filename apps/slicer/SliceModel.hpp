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

#include "model/Model.hpp"
#include "core/Object.h"
#include "glm/glm.hpp"
#include <core/Help.h>

class Model;

namespace slicer
{
	
class SliceModel
{
  public:
	SliceModel(const Model*);
	SliceModel(const SliceModel& orig)  = delete;
	virtual ~SliceModel() {};
	
	IRunnable::ExecResult load(string& incoming);
	
	void render(bool resetTimer, bool draw_normals=false);
   
   IRunnable::ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
   void _help(Help&);
	
	string file() const;
	
	const Model* getModel() const { return pmodel; }
	
  private:
	glm::vec3 translate;
	glm::vec3 rotate;
	const Model* pmodel;
};

}
