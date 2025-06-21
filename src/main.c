#include <math.h>
#include "raylib.h"

int playerfov = 90;
float playerAngle = 0;

typedef struct v2
{
	float x, y;
} v2;
typedef struct wall
{
	v2 startPos;
	v2 endPos;
	Color color;
} wall;

#define SCREEN_HEIGHT 1920
#define SCREEN_WIDTH 1080

float angles[SCREEN_WIDTH];

v2 playerLocation = {200.0, 200.0};

v2 getIntersectionOfVectors(v2 a0, v2 a1, v2 b0, v2 b1)
{
	const float d = ((a0.x - a1.x) * (b0.y - b1.y)) - ((a0.y - a1.y) * (b0.x - b1.x));
	if (fabsf(d) < 0.000001f)
	{
		return (v2){NAN, NAN};
	}

	const float t = (((a0.x - b0.x) * (b0.y - b1.y)) - ((a0.y - b0.y) * (b0.x - b1.x))) / d,
				u = (((a0.x - b0.x) * (a0.y - a1.y)) - ((a0.y - b0.y) * (a0.x - a1.x))) / d;

	return (t >= 0 && t <= 1 && u >= 0 && u <= 1) ? ((v2){
														a0.x + (t * (a1.x - a0.x)),
														a0.y + (t * (a1.y - a0.y))})
												  : ((v2){NAN, NAN});
}

void initializeRotations()
{
	float step = (tan(playerfov / 2) * 2) / (SCREEN_WIDTH - 1);

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		float pos = (-1 + i * step);
		angles[i] = atan(pos);
	}
}

int main(void)
{
	v2 v10;
	v10.x = 500;
	v10.y = 500;

	v2 v11;
	v11.x = 600;
	v11.y = 600;

	v2 v20;
	v20.x = 100;
	v20.y = 500;

	v2 v21;
	v21.x = 1400;
	v21.y = 700;
	wall wall1 = {(v2){200, 200}, (v2){400, 200}};

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SCREEN_HEIGHT, SCREEN_WIDTH, "vEngine");
	initializeRotations();

	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		playerAngle += GetMouseDelta().x * GetFrameTime();
		if (IsKeyDown(KEY_W))
		{
			playerLocation.x += cos(playerAngle);
			playerLocation.y += sin(playerAngle);
		}
		// drawing
		BeginDrawing();

		ClearBackground(BLACK);
		// DrawFPS(0,0);

		DrawLine(wall1.startPos.x, wall1.startPos.y, wall1.endPos.x, wall1.endPos.y, BLUE);

		// DrawText(intersectText, 0, 0, 100, WHITE);

		for (int i = 0; i < SCREEN_WIDTH; i++)
		{
			float dx = cos(angles[i] + playerAngle) * 100;
			float dy = sin(angles[i] + playerAngle) * 100;
			DrawLine(playerLocation.x, playerLocation.y, playerLocation.x + dx, playerLocation.y + dy, PINK);
			if (getIntersectionOfVectors(playerLocation, (v2){playerLocation.x + dx, playerLocation.y + dy}, wall1.startPos, wall1.endPos).x != NAN)
			{
				v2 ints = getIntersectionOfVectors(playerLocation, (v2){dx, dy}, wall1.startPos, wall1.endPos);
				DrawCircle(ints.x, ints.y, 1, RED);
			}
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
