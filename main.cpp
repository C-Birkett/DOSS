#include <raylib.h>
#include <raymath.h>

#include "planets.h"

// consts
const char* WINDOW_TITLE = "DOSS";
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr unsigned int TARGET_FPS = 60;

constexpr Vector3 ORIGIN = (Vector3){0.0f, 0.0f, 0.0f};
constexpr Vector3 UP_VECTOR = (Vector3){0.0f, 0.0f, 1.0f};

constexpr Vector3 CAM_INITIAL_POSITION = (Vector3){5.0f, 3.0f, 10.0f};
constexpr float CAM_FOV = 45.0f;

// statics
static Camera3D GCamera = {0};
static Planets solarsystem;

// functions
void Update();
void HandleInputs();
void UpdateDrawFrame();

int main(int argc, char** argv)
{
    // initialise
    InitWindow(SCREEN_WIDTH,
               SCREEN_HEIGHT,
               WINDOW_TITLE);

    DisableCursor();

    SetTargetFPS(TARGET_FPS);

    // setup camera
    GCamera.position = CAM_INITIAL_POSITION;
    GCamera.target = ORIGIN;
    GCamera.up = UP_VECTOR;
    GCamera.fovy = CAM_FOV;
    GCamera.projection = CAMERA_PERSPECTIVE;

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
    
    solarsystem.Update(time);
}

void HandleInputs()
{
    UpdateCamera(&GCamera, CAMERA_FREE);
    
    // reset camera
    if(IsKeyPressed('Z'))
    {
        GCamera.position = CAM_INITIAL_POSITION;
        GCamera.target = ORIGIN;
    }
}

void UpdateDrawFrame()
{
    BeginDrawing();

        ClearBackground(BLACK);

        //DrawText("nice", 190, 200, 20, LIGHTGRAY);

        BeginMode3D(GCamera);

            solarsystem.DrawPlanets();

        EndMode3D();

        DrawFPS(10, 10);

    EndDrawing();
}
