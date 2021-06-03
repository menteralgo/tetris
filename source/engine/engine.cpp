#include "engine.hpp"

namespace gn
{
    Input::Input()
    {
        char keycode = 'A';
        char str[] = {' ', '\0'};

        for (int i = 0; i < 26; ++i)
        {
            char *key = (char *)malloc(2 * sizeof(char));
            str[0] = keycode++;
            keys[std::string(str)] = 0;
        }

        keys["ArrowUp"] = 0;
        keys["ArrowDown"] = 0;
        keys["ArrowLeft"] = 0;
        keys["ArrowRight"] = 0;
    }

    void Input::setKey(const char *code, bool down)
    {
        keys[code] = down ? 1 : 2;
    }

    bool Input::getKeyDown(std::string code)
    {
        int value = keys[code];
        return value == 1;
    }

    bool Input::getKeyUp(std::string code)
    {
        bool isUp = keys[code] == 2;
        keys[code] = 0;

        return isUp;
    }

    void Input::clear()
    {
        std::for_each(keys.begin(), keys.end(), [](std::pair<std::string, char> pair)
                      { pair.second = 0; });
    }

    void App::init(config cfg, SDL_Renderer *renderer, SDL_Window *window)
    {
        _cfg = cfg;
        _renderer = renderer;
        _window = window;
    }

    void App::addRenderer(RenderComponent *render_comp)
    {
        auto it = renderers.begin();
        while (it != renderers.end())
        {
            if ((*it)->order >= render_comp->order)
            {
                renderers.insert(it, render_comp);
                return;
            }
            ++it;
        }

        renderers.push_back(render_comp);
    }

    void App::removeRenderer(RenderComponent *render_comp)
    {
        renderers.remove(render_comp);
    }

    void App::addActor(Actor *actor)
    {
        actors.push_back(actor);
    }

    void App::removeActor(Actor *actor)
    {
        actors.remove(actor);
    }

    float App::getScreenX(float x)
    {
        return x;
    }

    float App::getScreenY(float y)
    {
        return y;
    }
}