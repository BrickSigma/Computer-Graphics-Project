#include "algorithms/line.hpp"

void Algorithms::draw_line(Vector2 p1, Vector2 p2, Color color)
{
    int x0 = p1.x;
    int y0 = p1.y;
    
    int x1 = p2.x;
    int y1 = p2.y;
    
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    int y = y0;          // Current y position
    int p = 2 * dy - dx; // Initial value of p
    for (int i = 0; i < dx + 1; i++)
    {
        DrawPixel(x0 + i, y, color);

        if (p >= 0)
        {
            y++; // Increase y
            p -= 2 * dx;
        }
        p += 2 * dy;
    }

}