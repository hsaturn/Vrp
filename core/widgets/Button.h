/* 
 * File:   WidButton.h
 * Author: hsaturn
 *
 * Created on 23 juin 2015, 23:32
 */

#ifndef WIDGETBUTTON_H
#define WIDGETBUTTON_H

#include "Draggable.hpp"
#include <StringUtil.hpp>

namespace hwidgets
{

	class Button : public Widget
	{
	  public:

		virtual ~Button() { };

		static Button* factory(string &data);
		
		void mouseClick(int button, int state, int x, int y) override;

	  protected:
		virtual long _render(long);

	  private:

		Button();;
		Button(const Button& orig);
		Button& operator=(const Button&) ;

		string text;
		bool left;
		const Color* color;
	} ;
}

#endif /* WIDBUTTON_H */

