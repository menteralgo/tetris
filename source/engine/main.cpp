#include <iostream>
#include "engine.hpp"

using namespace gn;

const float DELAY = 1000.f / 60.f;
const float DELTA_TIME = DELAY / 1000.f;

void gnInitialize(config config);

uint32_t gnRun();

void gnRender();

int main(int argc, char *atgv[])
{
    config cfg = { "Tetris", 0, 400, 500 };
    gnInitialize(cfg);

    OnStart();
    gnRun();

    return 0;
}

void gnInitialize(config config)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL Initialization error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    Uint32 window_flags = config.isFullScreen ? SDL_WINDOW_FULLSCREEN : 0;
    SDL_Window *window = SDL_CreateWindow(config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          config.screenWidth, config.screenHeight, window_flags | SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << "SDL window creation error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "SDL rendere creation error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    App::instance().init(config, renderer, window);
}

uint32_t gnRun()
{
    while (true)
    {
        gnInput();

        for (Actor *actor : App::instance().getActors())
            actor->update(DELTA_TIME);

        gnRender();

        Input::instance().clear();
    }

    return 0;
}

void gnRender()
{
    SDL_SetRenderDrawColor(App::instance().getSdlRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(App::instance().getSdlRenderer());

    std::list<RenderComponent*> rcomps = App::instance().getRenderComponents();
    for (const auto &rc : rcomps)
    {
        if (rc->isVisible)
            rc->render();
    }

    SDL_RenderPresent(App::instance().getSdlRenderer());
    SDL_Delay(DELAY);
}