#include <raylib.h>
#include <raymath.h>

#include "planets.h"

const int screenWidth = 800;
const int screenHeight = 600;

void Update();
void HandleInputs();
void UpdateDrawFrame();

static Camera3D camera = {0};

Planets solarsystem;

int main(int argc, char** argv)
{
    // initialise
    InitWindow(screenWidth, screenHeight, "DOSS");

    DisableCursor();

    SetTargetFPS(60);

    // setup camera
    camera.position = (Vector3){5.0f, 3.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 0.0f, 1.0f};
    camera.fovy = 45.0f;
    //camera.projection = CAMERA_ORTHOGRAPHIC;
    camera.projection = CAMERA_PERSPECTIVE;

    // main loop
    while(!WindowShouldClose())
    {
        Update();
        HandleInputs();
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

void Update()
{
    float time = GetFrameTime() * 1000000.0f; // run at 1000x speed
                                              //
    solarsystem.Update(time);
}

void HandleInputs()
{
    UpdateCamera(&camera, CAMERA_FREE);
    
    if(IsKeyPressed('Z')) camera.target = (Vector3){0.0f,0.0f,0.0f};
}

void UpdateDrawFrame()
{
    BeginDrawing();

        ClearBackground(BLACK);

        //DrawText("nice", 190, 200, 20, LIGHTGRAY);

        BeginMode3D(camera);

            solarsystem.DrawPlanets();

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}
