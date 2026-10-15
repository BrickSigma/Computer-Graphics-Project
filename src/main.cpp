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

// Using a globally filled polygon (not a good idea, but it works for the sake of it)
RenderTexture2D filled_polygon;

// Used to draw a polygon outline clipped to a box
void draw_polygon_filled()
{
    DrawTexture(filled_polygon.texture, 0, 0, WHITE);
}

/*
 * Draws a quadratic Bezier curve using small line segments.
 * This lets us position the curve wherever we need it.
 */
void draw_quadratic_curve(Vector2 start, Vector2 control,
                          Vector2 end, Color color)
{
    const int segments = 30;
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

        // Use the existing line drawing algorithm.
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
    Vector2 centre = {200, 200};

    // 1. Pink ball body
    Algorithms::draw_circle_filled(centre, 140.0f, PINK);

    // 2. Curved seams across the ball
    draw_quadratic_curve(
        {96, 144}, {200, 96}, {304, 144}, DARKBLUE);

    draw_quadratic_curve(
        {96, 256}, {200, 304}, {304, 256}, DARKBLUE);

    // 3. Straight lines forming the side patterns
    Algorithms::draw_line(
        {94, 176}, {164, 192}, DARKBLUE);

    Algorithms::draw_line(
        {94, 224}, {164, 208}, DARKBLUE);

    Algorithms::draw_line(
        {306, 176}, {236, 192}, DARKBLUE);

    Algorithms::draw_line(
        {306, 224}, {236, 208}, DARKBLUE);

    // 4. Hexagonal polygon in the centre
    Vector2 panel[] = {
        {200, 156},
        {234, 180},
        {234, 220},
        {200, 244},
        {166, 220},
        {166, 180}
    };

    size_t count = sizeof(panel) / sizeof(panel[0]);

    Algorithms::draw_polygon_outline(
        panel, count, MAROON);

    // 5. Outer circular border
    Algorithms::draw_circle_outline(
        centre, 140.0f, DARKBLUE);
}

RenderTexture2D canvas;

int main(int argc, char *argv[])
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics Rendering Algorithms Showcase");

	// Create a rendering canvas for drawing the algorithms to. Set as GRID_SIZE x GRID_SIZE pixels
	// Make sure to unload later!!!
	canvas = LoadRenderTexture(GRID_SIZE, GRID_SIZE);

	filled_polygon = LoadRenderTexture(GRID_SIZE, GRID_SIZE);
	Vector2 star[] = {{100, 15}, {125, 80}, {190, 85}, {140, 125}, {160, 190},
					  {100, 150}, {40, 190}, {60, 125}, {10, 85}, {75, 80}};
    size_t  count = sizeof(star) / sizeof(star[0]);
	Algorithms::draw_polygon_filled(filled_polygon, star, count, PINK);

	// Register all of the drawing functions here using the callback above
	DrawTest tests[NO_TESTS] = {
		{draw_line, "Line"},
		{draw_polygon, "Polygon Outline"},
		{draw_polygon_clipped, "Polygon Clip"},
		{draw_curve, "Curve"},
		{draw_bezier, "Bezier Curve"},
		{draw_circle_outline, "Circle Outline"},
		{draw_circle_filled, "Circle Filled"},
		{draw_ellipse_outline, "Ellipse Outline"},
		{draw_ellipse_filled, "Ellipse Filled"},
		{draw_polygon_filled, "Filled Polygon"},
		{NULL, "Algorithm 11"},
		{NULL, "Algorithm 12"},
	};

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		size_t test_no = 0;
		for (int row = 0; row < GRID_ROWS; row++)
		{
			for (int col = 0; col < GRID_COLS; col++)
			{
				if (test_no >= NO_TESTS)
				{
					break;
				}

				// Start rendering to the canvas
				BeginTextureMode(canvas);
				ClearBackground(WHITE); // Clear it as white
				if (tests[test_no].cb != NULL)
					tests[test_no].cb(); // Draw the algorithm to the canvas
				EndTextureMode();

				// Draw the background title
				DrawRectangle((col * (GRID_SIZE + GRID_SPACING)),
							  row * (GRID_SIZE + GRID_TITLE + GRID_SPACING),
							  GRID_SIZE,
							  GRID_TITLE,
							  LIGHTGRAY);

				// Render the algorithm title
				DrawText(tests[test_no].title,
						 (col * (GRID_SIZE + GRID_SPACING) + 8),
						 row * (GRID_SIZE + GRID_TITLE + GRID_SPACING) + 16, 20, BLACK);

				// Draw the rendered algorithm
				DrawTextureRec(canvas.texture,
							   {0, 0, GRID_SIZE, -GRID_SIZE},
							   {static_cast<float>(col * (GRID_SIZE + GRID_SPACING)),
								static_cast<float>(row * (GRID_SIZE + GRID_TITLE + GRID_SPACING) + GRID_TITLE)},
							   WHITE);

				test_no++;
			}
		}

    draw_ball();
		EndDrawing();
	}

	// Release any allocated objects, like the RenderTexture
	UnloadRenderTexture(canvas);
	UnloadRenderTexture(filled_polygon);

	CloseWindow();

	return 0;
}
