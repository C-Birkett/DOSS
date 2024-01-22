#pragma once

#include <raylib.h>

#include <vector>

struct PhysicsObject
{
    Vector3 velocity = {0.0f, 0.0f, 0.0f};
    Vector3 acceleration = {0.0f, 0.0f, 0.0f};
};

class Planets
{
public:
    Planets();
    ~Planets() = default;

    void Init(bool randomSystem = false, unsigned int numPlanets = 0);

    void Update(float timePassed);
    void DrawPlanets();

private:
    std::vector<unsigned int> parents;
    std::vector<Vector3> positions;
    std::vector<float> radii;
    std::vector<PhysicsObject> physicsObjects;

    void InitSolarSystem();
    void InitRandomSystem(unsigned int numPlanets);

    void UpdatePhysicsObjects(float timePassed);
    void UpdatePositions(float timePassed);

    Vector3 GetPlanetAccel(std::size_t planet);
};
