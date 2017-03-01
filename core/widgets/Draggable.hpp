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
	class Draggable : public Widget
	{
	  public:
		Draggable();

		typedef enum
		{
			STATE_IDLE,
			STATE_MOVING
		} enuState;

		virtual void mouseClick(Event &) final override;
		virtual void mouseMove(Event &) final override;

	  private:

		Draggable(const Draggable& orig);
		Draggable& operator=(const Draggable&) ;

		string text;
		bool left;
		const Color* color;

		enuState state;
		int offset_x;
		int offset_y;
	} ;

}

#endif /* WIDGETDRAGGABLE_HPP */

