#include <math.h>
#include "tetris.hpp"

namespace tet
{
    static uint8_t field[FIELD_WIDTH][FIELD_HEIGHT];
    static uint8_t buffer[FIELD_WIDTH][FIELD_HEIGHT];
    static uint8_t nextPolyBuffer[NPB_WIDTH][NPB_HEIGHT];

    static Polyomino polies[]{
        Polyomino(1, {{0, 0}, {1, 0}, {-1, 0}, {-2, 0}}),
        Polyomino(2, {{0, 0}, {1, 0}, {-1, 0}, {-1, -1}}),
        Polyomino(3, {{0, 0}, {-1, 0}, {1, 0}, {1, -1}}),
        Polyomino(4, {{0, 0}, {0, -1}, {1, 0}, {1, -1}}),
        Polyomino(5, {{0, 0}, {-1, 0}, {0, -1}, {1, -1}}),
        Polyomino(6, {{0, 0}, {-1, 0}, {0, -1}, {1, 0}}),
        Polyomino(7, {{0, 0}, {1, 0}, {-1, -1}, {0, -1}})};

    static vectorInt2 polyPosition;
    static Polyomino currentPoly;
    static int nextPolyIndex;
    static tetris_state state;

    static bool testPolyMove(vectorInt2 position, const Polyomino &poly);
    static bool testPolyRotate(vectorInt2 position, const Polyomino &poly);
    static void spawnPoly();
    static void movePolyDown();
    static void getNextPoly();

    Polyomino::Polyomino(uint8_t type, std::vector<vectorInt2> data)
    {
        coords = data;
        this->type = type;
    }

    void Polyomino::rotate()
    {
        static const double angle = -M_PI / 2.f;

        for (vectorInt2 &v : coords)
        {
            double x = static_cast<double>(v.x);
            double y = static_cast<double>(v.y);

            double x1 = x * cos(angle) - y * sin(angle);
            double y1 = x * sin(angle) + y * cos(angle);

            v.x = static_cast<uint32_t>(round(x1));
            v.y = static_cast<uint32_t>(round(y1));
        }
    }

    void initTetris()
    {
        srand(time(NULL));

        for (int y = 0; y < FIELD_HEIGHT; ++y)
        {
            for (int x = 0; x < FIELD_WIDTH; ++x)
            {
                field[x][y] = 0;
            }
        }

        getNextPoly();

        state = spawnPoly;
        state();
    }

    vectorInt2 putPolyomino(vectorInt2 position, const Polyomino &poly, uint8_t dest[][FIELD_HEIGHT])
    {
        int testX = 0;
        for (const auto &v : poly.coords)
        {
            int x = position.x + v.x;
            if (x < 0)
            {
                if (x < testX)
                    testX = x;
            }
            else if (x > FIELD_WIDTH - 1)
            {
                x -= FIELD_WIDTH - 1;
                if (x > testX)
                    testX = x;
            }
        }

        if (testX)
            position.x -= testX;

        for (const auto &v : poly.coords)
        {
            vectorInt2 p = position + v;
            if (p.y >= 0 && p.y < FIELD_HEIGHT)
                dest[p.x][p.y] = poly.type;
        }

        return position;
    }

    uint8_t getCell(int x, int y)
    {
        return buffer[x][y];
    }

    uint8_t getNextPolyCell(int x, int y)
    {
        return nextPolyBuffer[x][y];
    }

    static void clearBuffer()
    {
        for (int y = 0; y < FIELD_HEIGHT; ++y)
        {
            for (int x = 0; x < FIELD_WIDTH; ++x)
            {
                buffer[x][y] = field[x][y];
            }
        }
    }

    void runTetris()
    {
        clearBuffer();
        state();
    }

    void movePolyLeft()
    {
        if (testPolyMove(polyPosition + vectorInt2(-1, 0), currentPoly))
        {
            clearBuffer();

            polyPosition = polyPosition + vectorInt2(-1, 0);
            putPolyomino(polyPosition, currentPoly, buffer);
        }
    }

    void movePolyRight()
    {
        if (testPolyMove(polyPosition + vectorInt2(1, 0), currentPoly))
        {
            clearBuffer();

            polyPosition = polyPosition + vectorInt2(1, 0);
            putPolyomino(polyPosition, currentPoly, buffer);
        }
    }

    void rotatePoly()
    {
        Polyomino poly = currentPoly;
        poly.rotate();

        if (testPolyRotate(polyPosition, poly))
        {
            currentPoly.rotate();

            clearBuffer();
            polyPosition = putPolyomino(polyPosition, currentPoly, buffer);
        }
    }

    static void moveLine(int fromY, int toY)
    {
        for (int x = 0; x < FIELD_WIDTH; ++x)
        {
            field[x][toY] = field[x][fromY];
            field[x][fromY] = 0;
        }
    }

    static int lineBlocksCount(int lineY)
    {
        int c = 0;
        for (int x = 0; x < FIELD_WIDTH; ++x)
        {
            if (field[x][lineY])
                ++c;
        }

        return c;
    }

    static void completeLine()
    {
        int y = FIELD_HEIGHT;
        while (--y >= 0)
        {
            int c = lineBlocksCount(y);
            if (c == 0)
                break;

            if (c == FIELD_WIDTH)
            {
                for (int x = 0; x < FIELD_WIDTH; ++x)
                    field[x][y] = 0;
            }
        }
    }

    static void lineFallDown(int lineY)
    {
        int targetY = lineY + 1;
        if (targetY >= FIELD_HEIGHT)
            return;
        
        int c = lineBlocksCount(targetY);
        if (c == 0)
        {
            moveLine(lineY, targetY);
            lineFallDown(targetY);
        }
    }

    static void linesFall()
    {
        int y = FIELD_HEIGHT;
        while (--y >= 0)
        {
            if (lineBlocksCount(y) != 0)
                lineFallDown(y);
        }
    }

    static bool testPolyMove(vectorInt2 position, const Polyomino &poly)
    {
        for (const auto &v : poly.coords)
        {
            vectorInt2 p = v + position;
            if (p.y >= FIELD_HEIGHT)
                return false;

            if (p.x < 0 || p.x >= FIELD_WIDTH)
                return false;

            if (p.y >= 0 && field[p.x][p.y])
                return false;
        }

        return true;
    }

    static bool testPolyRotate(vectorInt2 position, const Polyomino &poly)
    {
        for (const auto &v : poly.coords)
        {
            vectorInt2 p = v + position;
            if (p.y >= FIELD_HEIGHT)
                return false;

            if (p.y >= 0 && field[p.x][p.y])
                return false;
        }

        return true;
    }

    static void getNextPoly()
    {
        nextPolyIndex = rand() % POLIES_NUM;
        Polyomino poly = polies[nextPolyIndex];

        for (int y = 0; y < NPB_HEIGHT; ++y)
        {
            for (int x = 0; x < NPB_WIDTH; ++x)
            {
                nextPolyBuffer[x][y] = 0;
            }
        }

        vectorInt2 position = vectorInt2(2, 1);
        for (const auto & v : poly.coords)
        {
            vectorInt2 p = v + position;
            nextPolyBuffer[p.x][p.y] = poly.type;
        }
    }

    static void spawnPoly()
    {
        polyPosition = vectorInt2(5, -1);

        currentPoly = polies[nextPolyIndex];
        getNextPoly();

        putPolyomino(polyPosition, currentPoly, buffer);

        state = movePolyDown;
    }

    static void movePolyDown()
    {
        polyPosition.y += 1;

        if (testPolyMove(polyPosition, currentPoly))
        {
            polyPosition = putPolyomino(polyPosition, currentPoly, buffer);
        }
        else
        {
            polyPosition.y -= 1;

            putPolyomino(polyPosition, currentPoly, field);
            putPolyomino(polyPosition, currentPoly, buffer);

            state = spawnPoly;
            
            completeLine();
            linesFall();
        }
    }
}