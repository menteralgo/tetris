#include <SDL2/SDL.h>
#include "engine.hpp"

namespace gn
{
    static char keyid[]{' ', '\0'};

    void gnParseKey(SDL_KeyboardEvent *event, bool keyDown)
    {
        if (event->repeat == 0)
        {
            int keycode = event->keysym.scancode;
            if (keycode >= SDL_SCANCODE_A && keycode <= SDL_SCANCODE_Z)
            {
                keyid[0] = 'A' + keycode - SDL_SCANCODE_A;
                Input::instance().setKey(keyid, keyDown);

                return;
            }

            switch (event->keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                    Input::instance().setKey("ArrowUp", keyDown);
                    break;
                case SDL_SCANCODE_DOWN:
                    Input::instance().setKey("ArrowDown", keyDown);
                    break;
                case SDL_SCANCODE_LEFT:
                    Input::instance().setKey("ArrowLeft", keyDown);
                    break;
                case SDL_SCANCODE_RIGHT:
                    Input::instance().setKey("ArrowRight", keyDown);
                    break;
            }
        }
    }

    void gnInput()
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
            {
                exit(0);
            }
            break;
            case SDL_KEYDOWN:
            {
                gnParseKey(&event.key, true);
            }
            break;
            case SDL_KEYUP:
            {
                gnParseKey(&event.key, false);
            }
            break;
        }
    }
}