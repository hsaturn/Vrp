#include "EventHandler.hpp"

namespace hwidgets
{

	map<EventHandler::EventHandlerHook, EventHandler::EventHandlerEntry> EventHandler::handlers;
	
	EventHandler::EventHandler() { }

	void EventHandler::connect(EventHandlerHook handler, Event::EventType & e)
	{
		EventHandlerEntry entry;
		entry.event_mask = e;
		entry.last_event.all = 0;
		handlers[handler] = entry;
	}

	void EventHandler::disconnect(EventHandlerHook handler)
	{
		auto it = handlers.find(handler);
		if (it != handlers.end())
			handlers.erase(it);
	}
	
	void EventHandler::dispatch(Event* e)
	{
		for(auto it : handlers)
		{
			if ((e->event.all & it.second.event_mask.all) != it.second.last_event.all)
			{
				it.first(e);
				it.second.last_event.all = e->event.all & it.second.event_mask.all;
			}
		}
	}
}