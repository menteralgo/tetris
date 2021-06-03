#pragma once

#include "gn.hpp"
#include "tetris.hpp"

using namespace gn;

namespace tet
{
    class Player : public Component
    {
    public:
        void update(float deltaTime) override;
    };

    class CellView : public Actor
    {
    public:
        Sprite *sprite;

        CellView(Sprite *sprite) : sprite(sprite) { this->sprite->setActor(this); }
    };

    class PolyCellView : public Actor
    {
    public:
        ColorSprite *_sprites[POLIES_NUM];

        PolyCellView(ColorSprite *sprites[POLIES_NUM]);
    };

    class TetrisView : public RenderComponent
    {
        PolyCellView *fieldView[FIELD_WIDTH][FIELD_HEIGHT];
        PolyCellView *nextPolyView[NPB_WIDTH][NPB_HEIGHT];
        CellView *backLines[FIELD_WIDTH];

        LineDrawer lineDrawer;
        ColorSprite *darkLineSprite;
        ColorSprite *lightLineSprite;

    public:
        TetrisView();

        void render() override;
    };

    class Tetris : public Actor
    {
        float time = 0;

    public:

        void update(float deltaTime) override;
    };
}