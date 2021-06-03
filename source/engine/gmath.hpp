#pragma once

namespace gn
{
    struct vector2
    {
        float x;
        float y;

        vector2() { }
        vector2(float x, float y) : x(x), y(y) { }

        vector2 operator+(vector2 v) const
        {
            return vector2(x + v.x, y + v.y);
        }

        vector2 operator-(vector2 v) const
        {
            return vector2(x - v.x, y - v.y);
        }
    };

    struct vectorInt2
    {
        int32_t x;
        int32_t y;

        vectorInt2() { }
        vectorInt2(int32_t x, int32_t y) : x(x), y(y) { }

        vectorInt2 operator+(vectorInt2 v) const
        {
            return vectorInt2(x + v.x, y + v.y);
        }

        vectorInt2 operator-(vectorInt2 v) const
        {
            return vectorInt2(x - v.x, y - v.y);
        }
    };
}