#include "Draggable.hpp"
#include <GL/freeglut_std.h>

namespace hwidgets
{
 	Draggable::Draggable()
	: state(STATE_IDLE) { }

	void Draggable::mouseClick(int button, int bstate, int x, int y)
	{
		bool btnDrag = button & GLUT_RIGHT_BUTTON;
		if (state == STATE_MOVING && (btnDrag == false || bstate != GLUT_DOWN))
		{
			state = STATE_IDLE;
			mouseRelease();
			// TODO raise widgetMoved();
		}
		else if (state == STATE_IDLE && (btnDrag && bstate == GLUT_DOWN))
		{
			offset_x = mrect->x1() - x;
			offset_y = mrect->y1() - y;
			state = STATE_MOVING;
			mouseCapture();
		}
		else
			_mouseClick(button, bstate, x, y);
	}

	void Draggable::mouseMove(int x, int y, Widget* parent)
	{
		if (state == STATE_MOVING)
		{
			mrect->setX(x + offset_x);
			mrect->setY(y + offset_y);
		}
		else
			_mouseMove(x, y, parent);
	}

}