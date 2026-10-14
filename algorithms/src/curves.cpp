
#include "algorithms/curves.hpp"
#include <cmath>

void Algorithms::draw_curve()
{
    const float pi = 3.14159265f;
    const int segments = 400;

    // Curve dimensions and position within the 200 x 200 canvas
    const float centerY = 100.0f;
    const float amplitude = 45.0f;
    const float frequency = 2.0f;

    for (int i = 0; i <= segments; i++)
    {
        float t = static_cast<float>(i) / segments;

        // Map t from [0, 1] to x coordinates [10, 190]
        float x = 10.0f + t * 180.0f;

        // Calculating the sine curve's y coordinate
        float angle = t * frequency * 2.0f * pi;
        float y = centerY - amplitude * std::sin(angle);

        DrawPixel(
            static_cast<int>(std::round(x)),
            static_cast<int>(std::round(y)),
            BLUE
        );
    }
}

void Algorithms::draw_bezier()
{
    // Four control points for a cubic Bezier curve.
    Vector2 p0 = {20.0f, 160.0f};
    Vector2 p1 = {45.0f, 20.0f};
    Vector2 p2 = {150.0f, 20.0f};
    Vector2 p3 = {180.0f, 160.0f};

    const int segments = 500;

    for (int i = 0; i <= segments; i++)
    {
        float t = static_cast<float>(i) / segments;
        float u = 1.0f - t;

        //  The cubic Bezier equation.
        float x = u * u * u * p0.x
                + 3.0f * u * u * t * p1.x
                + 3.0f * u * t * t * p2.x
                + t * t * t * p3.x;

        float y = u * u * u * p0.y
                + 3.0f * u * u * t * p1.y
                + 3.0f * u * t * t * p2.y
                + t * t * t * p3.y;

        DrawPixel(
            static_cast<int>(std::round(x)),
            static_cast<int>(std::round(y)),
            RED
        );
    }
}

void Algorithms::draw_spline()
{
    // Implementation of another curve-drawing implementation.
}
