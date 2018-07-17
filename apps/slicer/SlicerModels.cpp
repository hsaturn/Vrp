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
	
	void cmdList(int index, SliceModel* model, ostream& out)
	{
		out << "Model #" << index << ' ' << model->file() << endl;
	}

	void cmdBox(int index, SliceModel* model, ostream& out)
	{
		cmdList(index, model, out);
		out << "BOX MIN" << model->getModel()->getMinCoord() << " MAX"  << model->getModel()->getMaxCoord() << endl;
	}
	
	Application::ExecResult SlicerModels::load(string &incoming)
	{
		Application::ExecResult result = Application::EXEC_FAILED;

		const Model* pmodel = Model::get(incoming);
		if (pmodel)
		{
			SliceModel* psm = new SliceModel(pmodel);
			models[models.size()] = psm;
		}

		return result;
	}

	void SlicerModels::render(bool resetTimer, bool normals)
	{
		for (auto sliceModels : models)
			sliceModels.second->render(resetTimer, normals);
	}

	Application::ExecResult SlicerModels::execute(Server* psvr, string cmd, string incoming, const string& org, CmdQueue&)
	{
		Application::ExecResult result = Application::EXEC_UNKNOWN;
		function<void(int idx, SliceModel* model, ostream & out) > each;
		stringstream out;
		bool beach = false;

		if (cmd == "list")
		{
			beach = true;
			each = cmdList;
			result = Application::EXEC_OK;
		}
		else if (cmd == "box")
		{
			beach = true;
			each = cmdBox;
			result = Application::EXEC_OK;
		}

		if (beach)
		{
			for (auto it : models)
				each(it.first, it.second, out);

			psvr->send(out.str());
			cout << out.str();
		}
		
		return result;
	}

}