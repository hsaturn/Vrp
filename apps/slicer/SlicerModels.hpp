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

#include "core/Object.h"
#include "SliceModel.hpp"

class Model;

namespace slicer
{

	class SlicerModels
	{
	  public:
		SlicerModels() {};
		
		Application::ExecResult load(string &incoming);
		Application::ExecResult unload(string &incoming);
		
		void render(bool resetTimer, bool draw_normals=false);
		
		Application::ExecResult execute(Server*, string cmd, string incoming, const string& org, CmdQueue&);
      
      // remove spaces special chars, append a number if needed etc.
      string buildUniqueName(string sFileName);
	
	  private:
		map<string, SliceModel*> models;
	};
}

