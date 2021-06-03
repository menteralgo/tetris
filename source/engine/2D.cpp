#include <cassert>

#include "engine.hpp"
#include "2D.hpp"

namespace gn
{
    Sprite::Sprite(Bounds *bounds, const char *filename) : _bounds(bounds)
    {
        texture = IMG_LoadTexture(App::instance().getSdlRenderer(), filename);
    }

    void Sprite::render()
    {
        SDL_Rect dest;
        App &app = App::instance();

        dest.x = (int)app.getScreenX(_bounds->point1().x + actor->position.x);
        dest.y = (int)app.getScreenY(_bounds->point1().y + actor->position.y);
        dest.w = (int)app.getScreenX(_bounds->size().x);
        dest.h = (int)app.getScreenX(_bounds->size().y);

        SDL_RenderCopy(App::instance().getSdlRenderer(), texture, NULL, &dest);
    }

    ColorSprite::ColorSprite(Bounds *bounds, uint32_t color)
    {
        _bounds = bounds;

        SDL_Renderer *renderer = App::instance().getSdlRenderer();

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, bounds->size().x, bounds->size().y);
        int error = SDL_SetRenderTarget(renderer, texture);

        assert(!error);

        error = SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), 0xFF);
        assert(!error);

        SDL_Rect rect = SDL_Rect { 0, 0, (int) bounds->size().x, (int) bounds->size().y };
        error = SDL_RenderFillRect(renderer, &rect);
        assert(!error);

        error = SDL_SetRenderTarget(renderer, NULL);
        assert(!error);
    }

    void LineDrawer::setLine(vectorInt2 p1, vectorInt2 p2, uint32_t color)
    {
        _p1 = p1;
        _p2 = p2;
        _color = color;
    }

    void LineDrawer::render()
    {
        SDL_SetRenderDrawColor(App::instance().getSdlRenderer(), R(_color), G(_color), B(_color), 0xFF);
        SDL_RenderDrawLine(App::instance().getSdlRenderer(), _p1.x, _p1.y, _p2.x, _p2.y);
    }
}