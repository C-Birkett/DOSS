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

    void Init(unsigned int numPlanets = 0);

    void Update(float timePassed);
    void DrawPlanets();

private:
    unsigned int m_numPlanets;
    std::vector<unsigned int> parents;
    std::vector<Vector3> positions;
    std::vector<float> masses;
    std::vector<float> forces;
    std::vector<float> radii;
    std::vector<PhysicsObject> physicsObjects;

    void InitSolarSystem();
    void InitRandomSystem(unsigned int numPlanets);

    void UpdatePhysicsObjects(float timePassed);
    void UpdatePositions(float timePassed);

    Vector3 GetRelativePosn(unsigned int planet);
    Vector3 GetPlanetAccel(unsigned int planet);
    float GetPlanetGravForce(unsigned int p1, unsigned int p2, float radius);
    float GetPlanetVolume(unsigned int planet);
};
