#pragma once

#include <raylib.h>

#include <vector>

struct PhysicsObject
{
    Vector3 velocity;
    Vector3 accelleration;
};

class Planets
{
public:
    Planets();
    ~Planets();

    std::vector<Vector3> positions;
    std::vector<float> radii;
    std::vector<PhysicsObject> physicsObjects;

    void InitSolarSystem();

    void UpdatePhysicsObjects(float timePassed);
    void UpdatePositions(float timePassed);
    void DrawPlanets();

    Vector3 GetPlanetAccel(std::size_t planet);
};
