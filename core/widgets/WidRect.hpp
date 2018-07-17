/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WidRect.hpp
 * Author: hsaturn
 *
 * Created on 24 août 2017, 09:43
 */

#ifndef WIDRECT_HPP
#    define WIDRECT_HPP
#    include <string>

using namespace std;

class Color;

namespace hwidgets
{
	class Coord;

	class WidRect
	{
	  public:

		static WidRect* factory(string &data, WidRect* wparent);

		bool inside(int x, int y) const
		{
			return x >= x1() && x <= x2() && y >= y1() && y <= y2();
		}

		bool render(bool highlight) const;

		int width() const;

		int height() const;


		int x1() const;
		int x2() const;
		int y1() const;
		int y2() const;

		void setX1(int x);
		void setY1(int y);
		void setX2(int x);
		void setY2(int y);

		/*
		void setX(int x) { x1 = x; };
		void setY(int y) { y1 = y; };
		void setWidth(int w) { x2 = x1 + w; }
		void setHeight(int h) { y2 = y1 + h;
		 */

	  private:

		WidRect(WidRect* rect_parent = 0) : parent(rect_parent) { };

		WidRect(const WidRect&);
		WidRect& operator =(const WidRect&);

		WidRect* parent;
		Coord* tl;
		Coord* wh;
		const Color* color;
	};
}



#endif /* WIDRECT_HPP */

