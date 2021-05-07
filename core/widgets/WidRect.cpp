/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WidRect.cpp
 * Author: hsaturn
 * 
 * Created on 24 août 2017, 09:43
 */

#include "WidRect.hpp"
#include "Widget.h"

namespace hwidgets
{

	int WidRect::width() const
	{
		return wh->getAbsoluteX();
	}

	int WidRect::height() const
	{
		return wh->getAbsoluteY();
	}

	int WidRect::x1() const
	{
		return tl->getAbsoluteX();
	}

	int WidRect::x2() const
	{
		return tl->getAbsoluteX() + wh->getAbsoluteX();
	}

	int WidRect::y1() const
	{
		return tl->getAbsoluteY();
	}

	int WidRect::y2() const
	{
		return tl->getAbsoluteY() + wh->getAbsoluteY();
	}

	void WidRect::setX1(int x)
	{
		tl->setAbsoluteX(x);
	}

	void WidRect::setY1(int y)
	{
		tl->setAbsoluteY(y);
	}

	void WidRect::setX2(int x)
	{
		wh->setAbsoluteX(x);
	}

	void WidRect::setY2(int y)
	{
		wh->setAbsoluteY(y);
	}

	WidRect* WidRect::factory(string &infos, WidRect* rect_parent)
	{
		WidRect* rect=nullptr;

		if (infos.substr(0, 4) == "help")
		{
			Widget::pushMessage("rectangle help");
			Widget::pushMessage(" top_left_coord bottom_right_coord color");
			Coord::factory(infos, rect_parent); // Help for coord
		}
		else
		{
			rect = new WidRect(rect_parent);
			rect->tl = Coord::factory(infos, rect_parent);
			rect->wh = Coord::factory(infos, rect_parent);
			rect->color = Color::factory(infos);
			if (rect->tl==0 || rect->wh==0)
			{
				cerr << "bad rectangle" << endl;
				delete rect;
				rect = nullptr;
			}
		}
		return rect;
	}

	bool WidRect::render(bool highlight) const
	{
		glBegin(GL_QUADS);
		const Color* col = color ? color : &Color::dark_gray;

		if (highlight)
		{
			col->renderHighlight(2);
		}
		else
			col->render();
		
		int x(tl->getAbsoluteX());
		int y(tl->getAbsoluteY());
		glVertex2i(x, y);
		glVertex2i(x + wh->getAbsoluteX(), y);
		glVertex2i(x + wh->getAbsoluteX(), y + wh->getAbsoluteY());
		glVertex2i(x, y + wh->getAbsoluteY());
		glEnd();
		return false;
	}


}
