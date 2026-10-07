#include <raylib.h>

int main(int argc, char *argv[])
{
	InitWindow(800, 800, "Game");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(WHITE);

			DrawText("Hello inside of Raylib!", 30, 30, 24, BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
