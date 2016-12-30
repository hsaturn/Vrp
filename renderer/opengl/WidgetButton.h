/* 
 * File:   WidButton.h
 * Author: hsaturn
 *
 * Created on 23 juin 2015, 23:32
 */

#ifndef WIDGETBUTTON_H
#define	WIDGETBUTTON_H

#include "Widget.h"
#include <StringUtil.hpp>

namespace hwidgets
{
	

class WidgetButton : public Widget {
public:
	virtual ~WidgetButton(){};
	
	static WidgetButton* factory(string &data);

protected:
	virtual long _render(long);
	
private:
	WidgetButton(){};
	WidgetButton(const WidgetButton& orig);
	WidgetButton& operator=(const WidgetButton&);
	
	string text;
	bool left;
	const Color* color;
};

}
#endif	/* WIDBUTTON_H */

