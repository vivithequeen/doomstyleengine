#include <math.h>
#include "raylib.h"



int playerfov = 90;
float playerLocation = 0.0;


float angles[1080];


struct v2
{
	float x, y;
};



struct v2 getIntersectionOfVectors(struct v2 a0,struct v2 a1 , struct v2 b0,struct v2 b1){
	const float d = ((a0.x - a1.x) * (b0.y - b1.y)) - ((a0.y-a1.y) * (b0.x-b1.x));
	if(fabsf(d) < 0.000001f) {return (struct v2){NAN,NAN};}

	const float t = (((a0.x - b0.x) * (b0.y - b1.y))
					- ((a0.y - b0.y) * (b0.x - b1.x))) / d,
				u = (((a0.x - b0.x) * (a0.y - a1.y))
					- ((a0.y - b0.y) * (a0.x - a1.x))) / d;
	
	return (t >= 0 && t<=1 && u >= 0 && u <=1) ?
	((struct v2) {
		a0.x + (t * (a1.x - a0.x)),
		a0.y + (t * (a1.y - a0.y))
	}) : ((struct v2) {NAN, NAN});
				
}

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
	struct v2 v10;
	v10.x = 500;
	v10.y =500;
	struct v2 v11;
	v11.x = 600;
	v11.y =600;

	struct v2 v20;
	v20.x = 100;
	v20.y =500;
	struct v2 v21;
	v21.x = 1400;
	v21.y =700;

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
		//DrawFPS(0,0);
		DrawLine(v10.x,v10.y,v11.x,v11.y,RED);
		DrawLine(v20.x,v20.y,v21.x,v21.y,GREEN);

		struct v2 intsersect = getIntersectionOfVectors(v10,v11,v20,v21);
		
		char intersectText[64];
		snprintf(intersectText, sizeof(intersectText), "%.2f", intsersect.x);
		DrawText(intersectText, 0, 0, 100, WHITE);
		DrawCircle((int)intsersect.x, (int)intsersect.y,3, BLUE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
