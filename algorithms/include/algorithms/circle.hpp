#ifndef ALGORITHMS_CIRCLE_HPP
#define ALGORITHMS_CIRCLE_HPP

#include <raylib.h>

namespace Algorithms
{
    /**
     * Draw the outline of a circle.
     */
    void draw_circle_outline(Vector2 center, float r, Color color);

    /**
     * Draw a filled in circle
     */
    void draw_circle_filled(Vector2 center, float r, Color color);

    /**
     * Draw the outline of an ellipse.
     *
     * @param center center of the ellipse
     * @param major length of the major axis
     * @param minor length of the minor axis
     * @param color color of the ellipse outline
     */
    void draw_ellipse_outline(Vector2 center, float major, float minor, Color color);

    /**
     * Draw a filled in ellipse
     *
     * @param center center of the ellipse
     * @param major length of the major axis
     * @param minor length of the minor axis
     * @param color color of the ellipse outline
     */
    void draw_ellipse_filled(Vector2 center, float major, float minor, Color color);
} // namespace Algorithms

#endif // ALGORITHMS_CIRCLE_HPP