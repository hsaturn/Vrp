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

class Model;

namespace slicer
{
	
class SliceModel
{
  public:
	SliceModel(const Model*);
	SliceModel(const SliceModel& orig)  = delete;
	virtual ~SliceModel() {};
	
	Application::ExecResult load(string& incoming);
	
	void render(bool resetTimer, bool draw_normals=false);
	
	string file() const;
	
	const Model* getModel() const { return pmodel; }
	
  private:
	glm::vec3 translate;
	glm::vec3 rotate;
	const Model* pmodel;
};

}
