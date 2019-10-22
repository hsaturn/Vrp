/* 
 * File:   WidButton.h
 * Author: hsaturn
 *
 * Created on 23 juin 2015, 23:32
 */

#pragma once
#include "Draggable.hpp"
#include <StringUtil.hpp>

namespace hwidgets
{

	class Button : public Widget
	{
	  public:

		virtual ~Button() { };

		static Button* factory(string &data);
		
		void mouseClick(Event&) override;

	  protected:
		long _render(long ms) override;

	  private:

		Button();;
		Button(const Button& orig);
		Button& operator=(const Button&) ;

		string text;
		bool left;
		const Color* color;
	} ;
}
