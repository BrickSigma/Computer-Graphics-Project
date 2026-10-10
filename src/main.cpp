#include <cstddef>
#include <raylib.h>

#include "algorithms.hpp"
#include "algorithms/polygon.hpp"

constexpr int GRID_ROWS = 2;
constexpr int GRID_COLS = 5;
constexpr int GRID_SPACING = 1;

// Let each grid be 200x200 pixels
constexpr int GRID_SIZE = 200;

// Size of title height
constexpr int GRID_TITLE = 64;

constexpr int SCREEN_WIDTH = (GRID_SIZE * GRID_COLS) + (GRID_SPACING * (GRID_COLS - 1));
constexpr int SCREEN_HEIGHT = ((GRID_SIZE + GRID_TITLE) * GRID_ROWS) + (GRID_SPACING * (GRID_ROWS - 1));

constexpr size_t NO_TESTS = 9;

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

int main(int argc, char *argv[])
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics Rendering Algorithms Showcase");

	// Create a rendering canvas for drawing the algorithms to. Set as GRID_SIZE x GRID_SIZE pixels
	// Make sure to unload later!!!
	RenderTexture canvas = LoadRenderTexture(GRID_SIZE, GRID_SIZE);

	// Register all of the drawing functions here using the callback above
	DrawTest tests[NO_TESTS] = {
		{draw_line, "Line"},
		{draw_polygon, "Polygon Outline"},
		{draw_polygon_clipped, "Polygon Clip"},
		{NULL, "Algorithm 4"},
		{NULL, "Algorithm 5"},
		{NULL, "Algorithm 6"},
		{NULL, "Algorithm 7"},
		{NULL, "Algorithm 8"},
		{NULL, "Algorithm 9"},
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

		EndDrawing();
	}

	// Release any allocated objects, like the RenderTexture
	UnloadRenderTexture(canvas);

	CloseWindow();

	return 0;
}
