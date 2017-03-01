/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventHandler.hpp
 * Author: hsaturn
 *
 * Created on 26 février 2017, 05:15
 */

#ifndef EVENTHANDLER_HPP
#    define EVENTHANDLER_HPP
#    include <map>
#    include "Event.hpp"

namespace hwidgets
{

	class EventHandler
	{
	  public:
		typedef void (*EventHandlerHook)(Event &);

		class EventHandlerEntry
		{
		  public:
			Event::Type event_mask;
		};

		static void connect(EventHandlerHook handler, const Event::Type& e);
		static void disconnect(EventHandlerHook handler);
		static void dispatch(Event &);

	  protected:
		EventHandler();

		static map<EventHandlerHook, EventHandlerEntry> handlers;
	};
}


#endif /* EVENTHANDLER_HPP */

