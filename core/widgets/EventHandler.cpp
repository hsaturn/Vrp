#include "EventHandler.hpp"

namespace hwidgets
{
	map<EventHandler::EventHandlerHook, EventHandler::EventHandlerEntry> EventHandler::handlers;
	
	EventHandler::EventHandler() { }

	void EventHandler::connect(EventHandlerHook handler, const Event::Type & e)
	{
		EventHandlerEntry entry;
		entry.event_mask = e;
		handlers[handler] = entry;
	}

	void EventHandler::disconnect(EventHandlerHook handler)
	{
		auto it = handlers.find(handler);
		if (it != handlers.end())
			handlers.erase(it);
	}
	
	void EventHandler::dispatch(Event &e)
	{
		for(const auto& it : handlers)
		{
			if (e.type & it.second.event_mask)
				it.first(e);
		}
	}
}