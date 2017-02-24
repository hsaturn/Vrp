/* 
 * File:   gfloat .hpp
 * Author: francois
 *
 * Created on 24 février 2017, 16:03
 */

#ifndef WIDGETDRAGGABLE_HPP
#define WIDGETDRAGGABLE_HPP

#include "Widget.h"
#include <StringUtil.hpp>

namespace hwidgets
{
	class WidgetDraggable : public Widget
	{
	  public:
		WidgetDraggable();

		typedef enum
		{
			STATE_IDLE,
			STATE_MOVING
		} enuState;

		void mouseClick(int button, int state, int x, int y) final override;
		void mouseMove(int x, int y, Widget* parent) final override;

		virtual void _mouseClick(int button, int state, int x, int y){};
		virtual void _mouseMove(int x, int y, Widget* parent){};
		
	  private:

		WidgetDraggable(const WidgetDraggable& orig);
		WidgetDraggable& operator=(const WidgetDraggable&) ;

		string text;
		bool left;
		const Color* color;

		enuState state;
		int start_x;
		int start_y;
	} ;

}

#endif /* WIDGETDRAGGABLE_HPP */

