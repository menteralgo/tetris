#include "objects.hpp"

namespace tet
{
    void Player::update(float deltaTime)
    {
        if (Input::instance().getKeyUp("ArrowLeft"))
        {
            movePolyLeft();
        }
        else if (Input::instance().getKeyUp("ArrowRight"))
        {
            movePolyRight();
        }
        else if (Input::instance().getKeyUp("ArrowUp"))
        {
            rotatePoly();
        }
        else if (Input::instance().getKeyUp("ArrowDown"))
        {
        }
    }

    PolyCellView::PolyCellView(ColorSprite *sprites[POLIES_NUM])
    {
        for (int i = 0; i < POLIES_NUM; ++i)
        {
            _sprites[i] = sprites[i];
            _sprites[i]->setActor(this);
        }
    }

    TetrisView::TetrisView()
    {
        ColorSprite *cellSprites[POLIES_NUM] = 
        {
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0xC0C0C0),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0x00C0C0),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0xC08000),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0xC0C000),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0x00C000),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0x8000C0),
            new ColorSprite(new Bounds(vector2(SQUARE_SIZE - 2, SQUARE_SIZE - 2), vector2(0.f, 0.f)), 0xC00000),
        };

        for (int i = 0; i < POLIES_NUM; ++i)
        {
            App::instance().removeRenderer(cellSprites[i]);
        }

        lightLineSprite = new ColorSprite(new Bounds(vector2(SQUARE_SIZE, SQUARE_SIZE * FIELD_HEIGHT), vector2(0.f, 0.f)), 0x151515);
        App::instance().removeRenderer(lightLineSprite);

        darkLineSprite = new ColorSprite(new Bounds(vector2(SQUARE_SIZE, SQUARE_SIZE * FIELD_HEIGHT), vector2(0.f, 0.f)), 0x101010);
        App::instance().removeRenderer(darkLineSprite);

        vector2 position = vector2(1.f, 1.f);
        for (int y = 0; y < FIELD_HEIGHT; ++y)
        {
            for (int x = 0; x < FIELD_WIDTH; ++x)
            {
                PolyCellView *pcv = new PolyCellView(cellSprites);
                pcv->position = position;

                fieldView[x][y] = pcv;
                position.x += SQUARE_SIZE;
            }

            position.x = 1.f;
            position.y += SQUARE_SIZE;
        }

        position = vector2(1.f, 1.f);
        vector2 shift = vector2(SQUARE_SIZE * FIELD_WIDTH + 25.f, 50.f);
        for (int y = 0; y < NPB_HEIGHT; ++y)
        {
            for (int x = 0; x < NPB_WIDTH; ++x)
            {
                PolyCellView *pcv = new PolyCellView(cellSprites);
                pcv->position = position + shift;

                nextPolyView[x][y] = pcv;
                position.x += SQUARE_SIZE;
            }

            position.x = 1.f;
            position.y += SQUARE_SIZE;
        }

        position = vector2(0, 0);
        for (int i = 0; i < FIELD_WIDTH; ++i)
        {
            CellView *cv;
            if (i % 2)
                cv = new CellView(lightLineSprite);
            else
                cv = new CellView(darkLineSprite);
            
            cv->position = position;
            position.x += SQUARE_SIZE;

            backLines[i] = cv;
        }
    }

    void TetrisView::render()
    {
        for (int i = 0; i < FIELD_WIDTH; ++i)
        {
            CellView *cv = backLines[i];
            cv->sprite->setActor(cv);
            cv->sprite->render();
        }

        for (int y = 0; y < FIELD_HEIGHT; ++y)
        {
            for (int x = 0; x < FIELD_WIDTH; ++x)
            {
                uint8_t cell = getCell(x, y);
                if (cell)
                {
                    Sprite *sprite = fieldView[x][y]->_sprites[cell - 1];
                    sprite->setActor(fieldView[x][y]);
                    sprite->render();
                }
            }
        }

        for (int y = 0; y < NPB_HEIGHT; ++y)
        {
            for (int x = 0; x < NPB_WIDTH; ++x)
            {
                uint8_t cell = getNextPolyCell(x, y);
                if (cell)
                {
                    Sprite *sprite = nextPolyView[x][y]->_sprites[cell - 1];
                    sprite->setActor(nextPolyView[x][y]);
                    sprite->render();
                }
            }
        }
    }

    void Tetris::update(float deltaTime)
    {
        Actor::update(deltaTime);

        time += deltaTime;
        if (time >= 0.3f)
        {
            runTetris();
            time = 0;
        }
    }
}