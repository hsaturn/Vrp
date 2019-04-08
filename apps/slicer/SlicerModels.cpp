/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SlicerModels.cpp
 * Author: hsaturn
 *
 * Created on 15 août 2017, 11:55
 */

#include "SlicerModels.hpp"
#include "SliceModel.hpp"
#include "model/Model.hpp"
#include <functional>
#include <sstream>

namespace slicer
{
	ostream& operator<< (ostream& out, glm::vec3 v)
	{
		out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
		return out;
	}

	void cmdList(const string& name, SliceModel* model, ostream& out)
	{
		out << "Model #" << name << ' ' << model->file() << endl;
	}

	void cmdBox(const string& name, SliceModel* model, ostream& out)
	{
		cmdList(name, model, out);
		out << "BOX MIN" << model->getModel()->getMinCoord() << " MAX"  << model->getModel()->getMaxCoord() << endl;
	}

   IRunnable::ExecResult SlicerModels::unload(string &incoming)
   {
		IRunnable::ExecResult result = Application::EXEC_FAILED;

      string name=getWord(incoming);
      if (models.find(name) != models.end())
      {
         models.erase(name);
         result = IRunnable::EXEC_OK;
      }
      return result;
   }

	IRunnable::ExecResult SlicerModels::load(string &incoming)
	{
		IRunnable::ExecResult result = Application::EXEC_FAILED;

		const Model* pmodel = Model::get(incoming);
		if (pmodel)
		{
			SliceModel* psm = new SliceModel(pmodel);
			models[buildUniqueName(pmodel->getShortName())] = psm;
         result = IRunnable::EXEC_OK;
		}

		return result;
	}

   string SlicerModels::buildUniqueName(string name)
   {
      auto pos=name.rfind('/');
      if (pos==string::npos) pos=name.rfind('\\');  // TODO should be generic
      if (pos != string::npos)
         name.erase(0,pos+1);
      if (name.length()==0) name="model";

      string basename=name;
      if (basename[basename.length()-1]!='_') basename += '_';

      int i=0;
      while (models.find(name)!=models.end())
      {
         i++;
         stringstream s;
         s << basename << i;
         name=s.str();
      }
      return name;
   }

	void SlicerModels::render(bool resetTimer, bool normals)
	{
		for (auto sliceModels : models)
      {
			sliceModels.second->render(resetTimer, normals);
      }
	}

	IRunnable::ExecResult SlicerModels::execute(Server* psvr, string cmd, string incoming, const string& org, CmdQueue& queue)
	{
		IRunnable::ExecResult result = IRunnable::EXEC_UNKNOWN;
		function<void(const string& name, SliceModel* model, ostream & out) > each;

		stringstream out;
		bool beach = false;

		if (cmd == "list")
		{
			beach = true;
			each = cmdList;
		}
		else if (cmd == "box")
		{
			beach = true;
			each = cmdBox;
		}

		if (beach)
		{
			for (auto it : models)
				each(it.first, it.second, out);

			psvr->send(out.str());
			cout << out.str();
			result = IRunnable::EXEC_OK;
		}

      if (result == IRunnable::EXEC_UNKNOWN)
      {
         string sModel(cmd);
         sModel = getWord(sModel,".");
         const auto &it=models.find(sModel);
         if (it != models.end())
         {
            getWord(cmd, ".");
            result = it->second->execute(psvr, cmd, incoming, org, queue);
         }
      }

		return result;
	}

}