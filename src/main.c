#include <math.h>
#include "raylib.h"



int playerfov = 90;
float playerLocation = 0.0;


float angles[];

void initializeRotations()
{
	float step = (tan(playerfov / 2) * 2) / GetScreenWidth();

	for (int i = 0; i < GetScreenWidth(); i++)
	{
		float pos = (-1 + i * step);
		angles[i] = atan(pos);
	}
}



int main(void)
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1920, 1080, "vEngine");
	
	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		ClearBackground(BLACK);
		DrawFPS(0,0);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
