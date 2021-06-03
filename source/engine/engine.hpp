#pragma once

#include <cstdint>
#include <stdlib.h>
#include <list>
#include <map>
#include <string>
#include <algorithm>

#include <SDL2/SDL.h>
#include "gobject.hpp"

namespace gn
{
    #define R(c) (((c) & 0xFF0000) >> 16)
    #define G(c) (((c) & 0xFF00) >> 8)
    #define B(c) ((c) & 0xFF)

    void OnStart();

    struct config
    {
        const char *title;
        bool isFullScreen;
        uint16_t screenWidth;
        uint16_t screenHeight;
    };

    class Input
    {
        std::map<std::string, char> keys;

        Input();

    public:
        static Input & instance()
        {
            static Input instance;
            return instance;
        }

        Input(Input const &) = delete;
        Input operator=(Input const &) = delete;

        void setKey(const char *code, bool down);

        bool getKeyDown(std::string code);

        bool getKeyUp(std::string code);

        void clear();
    };

    class App
    {
        SDL_Renderer *_renderer;
        SDL_Window *_window;
        config _cfg;

        std::list<RenderComponent *> renderers;
        std::list<Actor *> actors;

        App() {}

    public:
        static App & instance()
        {
            static App instance;
            return instance;
        }

        App(App const &) = delete;
        App operator=(App const &) = delete;

        SDL_Renderer *getSdlRenderer() { return _renderer; }
        SDL_Window *getSdlWindow() { return _window; }
        const config &getConfig() { return _cfg; }
        const std::list<RenderComponent *> &getRenderComponents() { return renderers; };
        const std::list<Actor*> &getActors() { return actors; }

        void init(config cfg, SDL_Renderer *renderer, SDL_Window *window);

        void addRenderer(RenderComponent *renderComp);

        void removeRenderer(RenderComponent *renderComp);

        void addActor(Actor *actor);

        void removeActor(Actor *actor);

        float getScreenX(float x);

        float getScreenY(float y);
    };

    void gnParseKey(SDL_KeyboardEvent *event, bool keyDown);
    void gnInput();
}