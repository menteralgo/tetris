#pragma once

#include "gn.hpp"

using namespace gn;

namespace tet
{
    #define FIELD_WIDTH 10
    #define FIELD_HEIGHT 20
    #define SQUARE_SIZE 25
    #define POLIES_NUM 7

    #define NPB_WIDTH 4
    #define NPB_HEIGHT 4

    typedef void (*tetris_state)();
    
    struct Polyomino
    {
        std::vector<vectorInt2> coords;
        uint8_t type;

        Polyomino() { }
        Polyomino(uint8_t type, std::vector<vectorInt2> data);
        void rotate();
    };

    void initTetris();

    vectorInt2 putPolyomino(vectorInt2 position, const Polyomino &poly, uint8_t dest[][FIELD_HEIGHT]);
    uint8_t getCell(int x, int y);
    uint8_t getNextPolyCell(int x, int y);

    void runTetris();

    void movePolyLeft();
    void movePolyRight();
    void rotatePoly();
}