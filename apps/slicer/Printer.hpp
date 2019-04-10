/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Printer.hpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 08:23
 */

#pragma once

#include "ostream"
#include <string>
#include <core/Object.h>
#include "SlicerModels.hpp"

using namespace std;

namespace slicer
{

	class SlicerApp;

	class Printer : public IRenderable, IRunnable
	{
      public:
         Printer(SlicerApp* parent, string config);
         ~Printer() override;

         IRunnable::ExecResult _execute(Server*, string cmd, string incoming, const string& org, CmdQueue&) override;
      	void _help(Help&)	 const override;
         
         bool _render(bool resetTimer) override;
		
         bool load(string);
         const string& name() const { return msName; }
         
      private:
         Printer(const Printer& orig);

      private:
         SlicerModels models;
         string msName;
	};

}
