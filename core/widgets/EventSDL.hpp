/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventSDL.hpp
 * Author: hsaturn
 *
 * Created on 25 février 2017, 02:49
 */

#ifndef EVENTSDL_HPP
#    define EVENTSDL_HPP

#    include "Event.hpp"

namespace hwidgets
{

	class EventSDL : public Event
	{
	  public:
		EventSDL();

		virtual ~EventSDL() { };
	  protected:
		virtual void _update() override;
	};
}

#endif /* EVENTSDL_HPP */

