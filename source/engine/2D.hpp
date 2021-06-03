#pragma once

#include "gobject.hpp"

namespace gn
{
    class Sprite : public RenderComponent
    {
    protected:
        SDL_Texture *texture;
        Bounds *_bounds;
    public:
        Sprite() { }
        Sprite(Bounds *bounds, const char *filename);

        void render() override;
    };

    class ColorSprite : public Sprite
    {
    public:
        ColorSprite(Bounds *bounds, uint32_t color);
    };

    class LineDrawer : public RenderComponent
    {
        vectorInt2 _p1, _p2;
        uint32_t _color;
    public:
        LineDrawer() { }

        void setLine(vectorInt2 p1, vectorInt2 p2, uint32_t color);

        void render() override;
    };
}