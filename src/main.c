#include <math.h>
#include <stdio.h>
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

wall allWalls[32];
v2 playerLocation = {0, 0};

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

void drawWall(int step)
{
	float dx = cos(angles[step] + playerAngle) * 100;
	float dy = sin(angles[step] + playerAngle) * 100;

	// DrawLine(playerLocation.x, playerLocation.y, playerLocation.x + dx, playerLocation.y + dy, PINK);
	for (int i = 0; i < 32; i++) {
		wall w = allWalls[i];
		if (getIntersectionOfVectors(playerLocation, (v2){playerLocation.x + dx, playerLocation.y + dy}, w.startPos, w.endPos).x == getIntersectionOfVectors(playerLocation, (v2){playerLocation.x + dx, playerLocation.y + dy}, w.startPos, w.endPos).x) // check for not NaN
		{
			v2 intersectionPoint = getIntersectionOfVectors(playerLocation, (v2){playerLocation.x + dx, playerLocation.y + dy}, w.startPos, w.endPos);

			float distance = sqrt((playerLocation.x - intersectionPoint.x) * (playerLocation.x - intersectionPoint.x) + (playerLocation.y - intersectionPoint.y) * (playerLocation.y - intersectionPoint.y)) * cos(angles[step]);

			int height = ((SCREEN_WIDTH) / (distance));

			// height = max(min(height, SCREEN_WIDTH * 32), 1);

			// height *= height/abs(height);

			int offset = (SCREEN_WIDTH / 2) - (height / 2);

			DrawRectangle(step * 2, offset, 2, height, w.color);
			// DrawCircle(intersectionPoint.x, intersectionPoint.y, 1, RED);
			return;
		}
	}

}
int main(void)
{

	allWalls[0] = (wall){(v2){0, 0}, (v2){10, 20}, RED};
	allWalls[1] = (wall){(v2){10, 20}, (v2){20, 25}, BLUE};
	allWalls[2] = (wall){(v2){20, 25}, (v2){20, 15}, PINK};
	allWalls[3] = (wall){(v2){20, 15}, (v2){0, 0}, GREEN};

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(SCREEN_HEIGHT, SCREEN_WIDTH, "vEngine");
	initializeRotations();
	HideCursor();
	DisableCursor();
	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		playerAngle += GetMouseDelta().x * GetFrameTime() * 0.7;
		if (IsKeyDown(KEY_W))
		{
			playerLocation.x += cos(playerAngle) * GetFrameTime() * 10;
			playerLocation.y += sin(playerAngle) * GetFrameTime() * 10;
		}
		if (IsKeyDown(KEY_S))
		{
			playerLocation.x -= cos(playerAngle) * GetFrameTime() * 10;
			playerLocation.y -= sin(playerAngle) * GetFrameTime() * 10;
		}
		// drawing
		BeginDrawing();

		ClearBackground(BLACK);
		DrawFPS(0,0);

		// DrawLine(wall1.startPos.x, wall1.startPos.y, wall1.endPos.x, wall1.endPos.y, BLUE);

		// DrawText(intersectText, 0, 0, 100, WHITE);

		for (int i = 0; i < SCREEN_WIDTH; i++)
		{
			drawWall(i);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
