#include "algorithms/line.hpp"

void Algorithms::draw_line(Vector2 p1, Vector2 p2, Color color)
{
    DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}