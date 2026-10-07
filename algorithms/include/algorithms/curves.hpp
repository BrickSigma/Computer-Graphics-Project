#ifndef ALGORITHMS_CURVES_HPP
#define ALGORITHMS_CURVES_HPP

#include <raylib.h>

namespace Algorithms
{
    /**
     * Draw a curve
     * 
     * @note I'm honestly not sure what parameters are needed for the curve,
     * guess we'll need to check later...
     */
    void draw_curve();

    /**
     * Draw a Bezier curve
     * 
     * @note I'm honestly not sure what parameters are needed for the Bezier curve,
     * guess we'll need to check later...
     */
    void draw_bezier();

    /**
     * Draw a spline
     * 
     * @note I'm honestly not sure what parameters are needed for a spline,
     * guess we'll need to check later...
     */
    void draw_spline();
} // namespace Algorithms

#endif // ALGORITHMS_CURVES_HPP