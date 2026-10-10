#include "algorithms/polygon.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdlib> // std::abs
#include <utility> // std:swap

namespace
{
    // Bresenham's line algorithim for all directions
    void plot_line(Vector2 p1, Vector2 p2, Color color)
    {
        int x0 = p1.x;
        int y0 = p1.y;
        int x1 = p2.x;
        int y1 = p2.y;

        // Steep line? Swap axes so we always walk along the longer axis
        bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
        if (steep)
        {
            std::swap(x0,y0);
            std::swap(x1,y1);
        }

        // Right-to-left? Swap endpoints so x always increases
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        int dy = std::abs(y1 - y0);
        int ystep = (y0 < y1) ? 1 : -1;

        int p = 2 * dy - x0;
        int y = y0;
        for (int x = x0; x <= x1; x++)
        {
            if (steep)
                DrawPixel(y, x, color);
            else
                DrawPixel(x, y, color);

            if (p >= 0)
            {
                y += ystep;
                p -= 2 * dx;
            }
            p += 2 * dy;
        }
    }

    constexpr int OUT_INSIDE = 0;
    constexpr int OUT_LEFT = 1;
    constexpr int OUT_RIGHT = 2;
    constexpr int OUT_TOP = 4;
    constexpr int OUT_BOTTOM = 8;

    int compute_outcode(float x, float y, float xmin, float ymin, float xmax, float ymax)
    {
        int code = OUT_INSIDE;
        if (x < xmin) code |= OUT_LEFT;
        else if (x > xmax) code |= OUT_RIGHT;
        if (y < ymin) code |= OUT_TOP;
        else if (y > ymax) code |= OUT_BOTTOM;
        return code;
    }
    // Trims the line (x1,y1)-(x2,y2) to the box.
    // Returns false if nothing of it is visible. Otherwise the coordinates
    // are updated in place (that's what the & references are for).
    bool clip_line(float &x1, float &y1, float &x2, float&y2, Rectangle bounds)
    {
        const float xmin = bounds.x;
        const float ymin = bounds.y;
        const float xmax = bounds.x + bounds.width - 1;
        const float ymax = bounds.y + bounds.height - 1;

        int code1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
        int code2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);

        while (true)
        {
            if ((code1 | code2) == 0)
                return true;
            if (code1 & code2)
                return false;
            // Pick an endpoint that is outside
            int out = code1 ? code1 : code2;
           float x = 0, y = 0;

          if (out & OUT_TOP)
          {
              x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
              y = ymin;
          }
          else if (out & OUT_BOTTOM)
          {
              x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
              y = ymax;

          }
          else if (out & OUT_RIGHT)
          {
              y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
              x = xmax;
          }
          else // OUT_LEFT
          {
              y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
              x = xmin;
          }

          // Replace the outside endpoint with the intersection, then re-test
          if (out == code1)
          {
              x1 = x; y1 = y;
              code1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
          }
          else
          {
              x2 = x; y2 = y;
              code2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
          }
        }
    }
}

void Algorithms::draw_polygon_outline(const Vector2 *points, size_t no_points, Color color)
{
    if (no_points < 2)
        return; // nothing to connect
    for (size_t i = 0; i < no_points; i++)
    {
        Vector2 a = points[i];
        Vector2 b = points[(i + 1) % no_points];
        plot_line(a, b, color);
    }
}

void Algorithms::draw_polygon_outline_clipped(const Vector2 *points, size_t no_points, Rectangle bounds, Color color)
{
    if (no_points < 2)
        return;
    for (size_t i = 0; i < no_points; i++){
        Vector2 a = points[i];
        Vector2 b = points[(i + 1) % no_points];

        float x1 = a.x, y1 = a.y;
        float x2 = b.x, y2 = b.y;

        if (clip_line(x1, y1, x2, y2, bounds))
            plot_line(Vector2{x1,y1}, Vector2{x2,y2}, color);
    }
}

void Algorithms::draw_polygon_filled(const Vector2 *points, size_t no_points, Color color)
{
    // Implement the polygon drawing algorithm here.
}

void Algorithms::draw_polygon_filled_clipped(const Vector2 *points, size_t no_points, Rectangle bounds, Color color)
{
    // Implement the polygon drawing algorithm here.

    // This function MUST implement clipping within the bounding box passed in.
}
