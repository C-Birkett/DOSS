#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 600;

void UpdateDrawFrame();

int main(int argc, char** argv)
{
    InitWindow(screenWidth, screenHeight, "Cool Window");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("nice", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
