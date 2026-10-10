#ifndef ALGORITHMS_LINE_HPP
#define ALGORITHMS_LINE_HPP

#include <raylib.h>

namespace Algorithms
{
    /**
     * Draw a 2D line with a pixel width of 1 on the screen using Bresenham's Line Algorithm.
     * 
     * @param p1 point 1
     * @param p2 point 2
     * @param color the color of the line
     */
    void draw_line(Vector2 p1, Vector2 p2, Color color);
} // namespace Algorithms

#endif // ALGORITHMS_LINE_HPP