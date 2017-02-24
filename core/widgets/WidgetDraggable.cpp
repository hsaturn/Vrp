#include "WidgetDraggable.hpp"
#include <GL/freeglut_std.h>

namespace hwidgets
{

	WidgetDraggable::WidgetDraggable()
	: state(STATE_IDLE) { }

	void WidgetDraggable::mouseClick(int button, int bstate, int x, int y)
	{
		bool btnDrag = button & GLUT_RIGHT_BUTTON;
		if (state == STATE_MOVING)
		{
			if (btnDrag == false || bstate != GLUT_DOWN)
			{
				state = STATE_IDLE;
				mouseRelease();
				// TODO raise widgetMoved();
			}
		}
		else if (state == STATE_IDLE)
		{
			if (btnDrag && bstate == GLUT_DOWN)
			{
				state = STATE_MOVING;
				start_x = x;
				start_y = y;
				state = STATE_MOVING;
				mouseCapture();
			}
		}
	}

	void WidgetDraggable::mouseMove(int x, int y, Widget* parent)
	{
		if (state == STATE_MOVING)
		{
			cout << "moving" << endl;
			mrect->setX(x);
			mrect->setY(y);

		}
	}

}