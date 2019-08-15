/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SlicerModels.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:55
 */
#pragma once

#include "SliceModel.hpp"
#include "core/Application.h"

class Model;

namespace slicer
{

	class SlicerModels
	{
	  public:
		SlicerModels() {};
		
		Application::ExecResult load(string &incoming);
		
		void render(bool resetTimer, bool draw_normals=false);
		
		Application::ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
	
	  private:
		map<int, SliceModel*> models;
	};
}

