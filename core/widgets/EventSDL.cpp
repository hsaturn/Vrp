#if 0
#include <SDL2/SDL_events.h>

#include "EventSDL.hpp"

namespace hwidgets
{

	EventSDL::EventSDL() { }

	void EventSDL::_update()
	{
		static SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					changed = true;
					if (event.type == SDL_KEYDOWN)
						keybd.state = keybd.DOWN;
					else
						keybd.state = keybd.UP;
					uint16_t mod = event.key.keysym.mod;
					keybd.mod = keybd.NONE;
					if (mod & KMOD_LSHIFT)	keybd.mod |= keybd.L_SHIFT;
					if (mod & KMOD_RSHIFT)	keybd.mod |= keybd.R_SHIFT;
					if (mod & KMOD_LCTRL)	keybd.mod |= keybd.L_CTRL;
					if (mod & KMOD_RCTRL)	keybd.mod |= keybd.R_CTRL;
					if (mod & KMOD_LALT)	keybd.mod |= keybd.L_ALT;
					if (mod & KMOD_RALT)	keybd.mod |= keybd.R_ALT;
					if (mod & KMOD_MODE)	keybd.mod |= keybd.R_ALT;
					if (mod & KMOD_LGUI)	keybd.mod |= keybd.WINDOW;
					if (mod & KMOD_RGUI)	keybd.mod |= keybd.MENU;
					keybd.mouse_x = mouse.x;
					keybd.mouse_y = mouse.y;
				}
				break;

				default:
					break;
			}
		}
	}
}
#endif