#include "Draggable.hpp"
#include "Event.hpp"
#include <GL/freeglut_std.h>

namespace hwidgets
{
 	Draggable::Draggable()
	: state(STATE_IDLE) { }

	void Draggable::mouseClick(Event* event)
	{
		Event::Mouse &mouse=event->mouse;
		bool btnDrag = mouse.buttons.right;
		if (btnDrag == false && state == STATE_MOVING)
		{
			state = STATE_IDLE;
			mouseRelease();
			// TODO raise widgetMoved();
		}
		else if (state == STATE_IDLE && btnDrag )
		{
			offset_x = mrect->x1() - mouse.x;
			offset_y = mrect->y1() - mouse.y;
			state = STATE_MOVING;
			mouseCapture();
		}
	}

	void Draggable::mouseMove(Event* event)
	{
		
		if (state == STATE_MOVING)
		{
			Event::Mouse &mouse = event->mouse;
			mrect->setX(mouse.x + offset_x);
			mrect->setY(mouse.y + offset_y);
		}
	}

}