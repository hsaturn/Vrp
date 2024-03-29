/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SliceModel.cpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:06
 */

#include "SliceModel.hpp"

#define middle(model,c) (-(model->getMinCoord().c+model->getMaxCoord().c)/2)

namespace slicer
{
	SliceModel::SliceModel(const Model* model)
	:
	pmodel(model)
	{
		translate.x = middle(model,x);
		translate.y = middle(model,y);
		translate.z = -model->getMinCoord().z;
	}

   IRunnable::ExecResult SliceModel::_execute(Server* svr, string cmd, string incoming, const string& org, CmdQueue& queue)
   {
      IRunnable::ExecResult result(IRunnable::EXEC_UNKNOWN);
      if (cmd == "translate")
      {
         svr->send("NYI SliceModel::translate");
      }
      return result;
   }

   void SliceModel::_help(Help& help) const
   {
      auto pc=help.pushClass("{model_name}");
      help.add("translate x[,y[,z]]");
   }

	bool SliceModel::_render(bool resetTimer)
	{
		// TODO translate rotate
		glPushMatrix();
		glTranslatef(translate.x, translate.y, translate.z);
		pmodel->render(false); // TODO
		glPopMatrix();
      return false;
	}

	string SliceModel::file() const
	{
		return pmodel->getFileName();
	}
}