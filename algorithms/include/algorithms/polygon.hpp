#ifndef ALGORITHMS_POLYGON_HPP
#define ALGORITHMS_POLYGON_HPP

#include <raylib.h>
#include <cstddef>
#include <vector>


namespace Algorithms
{
    /**
     * Draw the outline/mesh of a polygon (one that isn't filled).
     * The implementation of this function doesn't need to handle clipping.
     * 
     * @param points an array of points to draw
     * @param no_points the length of the array
     * @param color the color of the polygon outline
     * 
     * @note if you want, you can use `std::vector` to pass the points in instead of
     * the C-style array, which can be called like so:
     * 
     * ```cpp
     * std::vector<Vector2> points;
	 * points.push_back(Vector2{.x=0, .y=0});
	 * points.push_back(Vector2{.x=1, .y=0});
	 * points.push_back(Vector2{.x=1, .y=1});
	 * points.push_back(Vector2{.x=0, .y=1});
     * Algorithms::draw_polygon_outline(points.data(), points.size(), BLACK);
     * ```
     */
    void draw_polygon_outline(const Vector2 *points, size_t no_points, Color color);

    /**
     * Draw the outline/mesh of a polygon (one that isn't filled) and clip the lines that go outside the screen.
     * 
     * @param points an array of points to draw
     * @param no_points the length of the array
     * @param bounds the bounding box of the polygon which is used to clip it
     * @param color the color of the polygon outline
     * 
     */
    void draw_polygon_outline_clipped(const Vector2 *points, size_t no_points, Rectangle bounds, Color color);

    /**
     * Draw a polygon that is filled in with color.
     * The implementation of this function doesn't need to handle clipping.
     * 
     * @param points an array of points to draw
     * @param no_points the length of the array
     * @param color the color of the polygon outline
     * 
     */
    void draw_polygon_filled(const Vector2 *points, size_t no_points, Color color);

    /**
     * Draw the outline/mesh of a polygon (one that isn't filled) and clip the lines that go outside the screen.
     * 
     * @param points an array of points to draw
     * @param no_points the length of the array
     * @param bounds the bounding box of the polygon which is used to clip it
     * @param color the color of the polygon outline
     * 
     */
    void draw_polygon_filled_clipped(const Vector2 *points, size_t no_points, Rectangle bounds, Color color);

} // namespace Algorithms

#endif // ALGORITHMS_POLYGON_HPP