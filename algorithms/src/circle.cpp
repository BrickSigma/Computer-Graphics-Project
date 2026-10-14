#include "algorithms/circle.hpp"
#include <cmath>

static void plot_circle_points(int cx, int cy, int x, int y, Color color)
{
    // 8-way symmetry for circles
    DrawPixel(cx + x, cy + y, color);
    DrawPixel(cx - x, cy + y, color);
    DrawPixel(cx + x, cy - y, color);
    DrawPixel(cx - x, cy - y, color);
    DrawPixel(cx + y, cy + x, color);
    DrawPixel(cx - y, cy + x, color);
    DrawPixel(cx + y, cy - x, color);
    DrawPixel(cx - y, cy - x, color);
}

static void plot_ellipse_points(int cx, int cy, int x, int y, Color color)
{
    // 4-way symmetry for ellipses
    DrawPixel(cx + x, cy + y, color);
    DrawPixel(cx - x, cy + y, color);
    DrawPixel(cx + x, cy - y, color);
    DrawPixel(cx - x, cy - y, color);
}

static void draw_horizontal_line(int x1, int x2, int y, Color color)
{
    for (int x = x1; x <= x2; x++)
    {
        DrawPixel(x, y, color);
    }
}

static void fill_circle_scanlines(int cx, int cy, int x, int y, Color color)
{
    // 8-way symmetry filled using horizontal scanlines
    draw_horizontal_line(cx - x, cx + x, cy + y, color);
    draw_horizontal_line(cx - x, cx + x, cy - y, color);
    draw_horizontal_line(cx - y, cx + y, cy + x, color);
    draw_horizontal_line(cx - y, cx + y, cy - x, color);
}

static void fill_ellipse_scanlines(int cx, int cy, int x, int y, Color color)
{
    // 4-way symmetry filled using horizontal scanlines
    draw_horizontal_line(cx - x, cx + x, cy + y, color);
    draw_horizontal_line(cx - x, cx + x, cy - y, color);
}

// 8 way symmetry for circles

void Algorithms::draw_circle_outline(Vector2 center, float r, Color color)
{
    int cx = static_cast<int>(std::round(center.x));
    int cy = static_cast<int>(std::round(center.y));
    int radius = static_cast<int>(std::round(r));

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y)
    {
        plot_circle_points(cx, cy, x, y, color);

        if (d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void Algorithms::draw_circle_filled(Vector2 center, float r, Color color)
{
    int cx = static_cast<int>(std::round(center.x));
    int cy = static_cast<int>(std::round(center.y));
    int radius = static_cast<int>(std::round(r));

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y)
    {
        fill_circle_scanlines(cx, cy, x, y, color);

        if (d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// 4 way symmetry for ellipses

void Algorithms::draw_ellipse_outline(Vector2 center, float major, float minor, Color color)
{
    int cx = static_cast<int>(std::round(center.x));
    int cy = static_cast<int>(std::round(center.y));
    int rx = static_cast<int>(std::round(major));
    int ry = static_cast<int>(std::round(minor));

    int x = 0;
    int y = ry;

    float rx2 = static_cast<float>(rx * rx);
    float ry2 = static_cast<float>(ry * ry);
    float two_rx2 = 2.0f * rx2;
    float two_ry2 = 2.0f * ry2;

    float dx = 0.0f;
    float dy = two_rx2 * y;

    // Region 1: Slope magnitude < 1 (step in x)
    float d1 = ry2 - (rx2 * ry) + (0.25f * rx2);
    while (dx < dy)
    {
        plot_ellipse_points(cx, cy, x, y, color);

        if (d1 < 0)
        {
            x++;
            dx += two_ry2;
            d1 += dx + ry2;
        }
        else
        {
            x++;
            y--;
            dx += two_ry2;
            dy -= two_rx2;
            d1 += dx - dy + ry2;
        }
    }

    // Region 2: Slope magnitude >= 1 (step in y)
    float d2 = (ry2 * (x + 0.5f) * (x + 0.5f)) +
               (rx2 * (y - 1.0f) * (y - 1.0f)) -
               (rx2 * ry2);

    while (y >= 0)
    {
        plot_ellipse_points(cx, cy, x, y, color);

        if (d2 > 0)
        {
            y--;
            dy -= two_rx2;
            d2 += rx2 - dy;
        }
        else
        {
            y--;
            x++;
            dx += two_ry2;
            dy -= two_rx2;
            d2 += dx - dy + rx2;
        }
    }
}

void Algorithms::draw_ellipse_filled(Vector2 center, float major, float minor, Color color)
{
    int cx = static_cast<int>(std::round(center.x));
    int cy = static_cast<int>(std::round(center.y));
    int rx = static_cast<int>(std::round(major));
    int ry = static_cast<int>(std::round(minor));

    int x = 0;
    int y = ry;

    float rx2 = static_cast<float>(rx * rx);
    float ry2 = static_cast<float>(ry * ry);
    float two_rx2 = 2.0f * rx2;
    float two_ry2 = 2.0f * ry2;

    float dx = 0.0f;
    float dy = two_rx2 * y;

    // Region 1
    float d1 = ry2 - (rx2 * ry) + (0.25f * rx2);
    while (dx < dy)
    {
        fill_ellipse_scanlines(cx, cy, x, y, color);

        if (d1 < 0)
        {
            x++;
            dx += two_ry2;
            d1 += dx + ry2;
        }
        else
        {
            x++;
            y--;
            dx += two_ry2;
            dy -= two_rx2;
            d1 += dx - dy + ry2;
        }
    }

    // Region 2
    float d2 = (ry2 * (x + 0.5f) * (x + 0.5f)) +
               (rx2 * (y - 1.0f) * (y - 1.0f)) -
               (rx2 * ry2);

    while (y >= 0)
    {
        fill_ellipse_scanlines(cx, cy, x, y, color);

        if (d2 > 0)
        {
            y--;
            dy -= two_rx2;
            d2 += rx2 - dy;
        }
        else
        {
            y--;
            x++;
            dx += two_ry2;
            dy -= two_rx2;
            d2 += dx - dy + rx2;
        }
    }
}
