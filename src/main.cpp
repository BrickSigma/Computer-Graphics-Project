#include <cstddef>
#include <raylib.h>

#include "algorithms.hpp"
#include "algorithms/polygon.hpp"

constexpr int GRID_ROWS = 2;
constexpr int GRID_COLS = 6;
constexpr int GRID_SPACING = 1;

// Let each grid be 200x200 pixels
constexpr int GRID_SIZE = 200;

// Size of title height
constexpr int GRID_TITLE = 64;

constexpr int SCREEN_WIDTH = (GRID_SIZE * GRID_COLS) + (GRID_SPACING * (GRID_COLS - 1));
constexpr int SCREEN_HEIGHT = ((GRID_SIZE + GRID_TITLE) * GRID_ROWS) + (GRID_SPACING * (GRID_ROWS - 1));

constexpr size_t NO_TESTS = 12;

// Drawing callback function type
typedef void (*draw_cb)();

// Custom struct used for drawing tests
typedef struct DrawTest
{
	draw_cb cb;		   // Callback to the draw function
	const char *title; // Title of the algorithm
} DrawTest;

// Used to draw a line
void draw_line()
{
	Algorithms::draw_line(Vector2{16, 16}, Vector2{185, 150}, RED);
}

// Used to a polygon outline
void draw_polygon()
{
    Vector2 points[] = {{40, 30}, {160, 50}, {180, 130}, {100, 175}, {30, 120}};
    size_t count = sizeof(points) / sizeof(points[0]); // number of elements
    Algorithms::draw_polygon_outline(points, count, BLUE);
}

// Used to draw a polygon outline clipped to a box
void draw_polygon_clipped()
{
    Rectangle bounds = {25,25,150,150};// modify the bounds to show clipping
    Vector2 star[] = {{100, 15}, {125, 80}, {190, 85}, {140, 125}, {160, 190},
					  {100, 150}, {40, 190}, {60, 125}, {10, 85}, {75, 80}};
    size_t  count = sizeof(star) / sizeof(star[0]);

    DrawRectangleLines((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height, LIGHTGRAY); // show the clip box
    Algorithms::draw_polygon_outline_clipped(star, count, bounds, RED);
}


void draw_curve()
{
    Algorithms::draw_curve();
}

void draw_bezier()
{
    Algorithms::draw_bezier();
}

// Used to draw a circle outline
void draw_circle_outline()
{
	Algorithms::draw_circle_outline(Vector2{100, 100}, 70.0f, BLUE);
}

// Used to draw a filled circle
void draw_circle_filled()
{
	Algorithms::draw_circle_filled(Vector2{100, 100}, 70.0f, PINK);
}

// Used to draw an ellipse outline
void draw_ellipse_outline()
{
	Algorithms::draw_ellipse_outline(Vector2{100, 100}, 80.0f, 45.0f, DARKGREEN);
}

// Used to draw a filled ellipse
void draw_ellipse_filled()
{
	Algorithms::draw_ellipse_filled(Vector2{100, 100}, 80.0f, 45.0f, PURPLE);
}


/*
 * Draws a quadratic Bezier curve using small line segments.
 * This lets us position the curve wherever we need it.
 */
void draw_quadratic_curve(Vector2 start, Vector2 control,
                          Vector2 end, Color color)
{
    // More segments produce smoother curves.
    const int segments = 180;
    Vector2 previous = start;

    for (int i = 1; i <= segments; i++)
    {
        float t = static_cast<float>(i) / segments;
        float u = 1.0f - t;

        Vector2 current = {
            u * u * start.x + 2.0f * u * t * control.x
                + t * t * end.x,
            u * u * start.y + 2.0f * u * t * control.y
                + t * t * end.y
        };

        Algorithms::draw_line(previous, current, color);
        previous = current;
    }
}

/*
 * Combines circles, lines, curves and a polygon
 * to create a patterned ball.
 */

void draw_ball()
{
    // Coordinates are multiplied by 3 for supersampling.
    Vector2 centre = {540, 540};

    // Pink ball body
    Algorithms::draw_circle_filled(centre, 420.0f, PINK);

    // Smooth curved seams
    draw_quadratic_curve(
        {288, 432}, {540, 288}, {792, 432}, DARKBLUE);

    draw_quadratic_curve(
        {288, 768}, {540, 912}, {792, 768}, DARKBLUE);

    // Straight panel lines
    Algorithms::draw_line(
        {282, 528}, {492, 576}, DARKBLUE);

    Algorithms::draw_line(
        {282, 672}, {492, 624}, DARKBLUE);

    Algorithms::draw_line(
        {918, 528}, {708, 576}, DARKBLUE);

    Algorithms::draw_line(
        {918, 672}, {708, 624}, DARKBLUE);

    // Central hexagonal polygon
    Vector2 panel[] = {
        {540, 468},
        {642, 540},
        {642, 660},
        {540, 732},
        {438, 660},
        {438, 540}
    };

    size_t count = sizeof(panel) / sizeof(panel[0]);

    Algorithms::draw_polygon_outline(
        panel, count, MAROON);

    // Outer border
    Algorithms::draw_circle_outline(
        centre, 420.0f, DARKBLUE);
}




int main()
{
    constexpr int CANVAS_SIZE = 360;
    constexpr int SCALE = 3;
    constexpr int RENDER_SIZE = CANVAS_SIZE * SCALE;

    InitWindow(CANVAS_SIZE, CANVAS_SIZE, "Pink Patterned Ball");
    SetTargetFPS(60);

    // Render at 3x resolution for smoother edges.
    RenderTexture canvas = LoadRenderTexture(
        RENDER_SIZE, RENDER_SIZE);

    SetTextureFilter(
        canvas.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose())
    {
        // Draw the ball on the high-resolution canvas.
        BeginTextureMode(canvas);
        ClearBackground(RAYWHITE);
        draw_ball();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Scale the rendered image down to 360x360.
        DrawTexturePro(
            canvas.texture,
            Rectangle{
                0, 0,
                static_cast<float>(RENDER_SIZE),
                -static_cast<float>(RENDER_SIZE)
            },
            Rectangle{
                0, 0,
                static_cast<float>(CANVAS_SIZE),
                static_cast<float>(CANVAS_SIZE)
            },
            Vector2{0, 0},
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();

    return 0;
}
