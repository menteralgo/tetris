#include "objects.hpp"

using namespace gn;
using namespace tet;

Tetris tetris;

void gn::OnStart()
{
    initTetris();

    tetris.addComponent(new Player());
    tetris.addComponent(new TetrisView());
}