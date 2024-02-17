#pragma once

#include "Vec3.h"

#include <vector>

struct PhysicsObject
{
    Vec3 velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    Vec3 acceleration = (Vector3){ 0.0f, 0.0f, 0.0f };
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
    std::vector<Vec3> positions;
    std::vector<double> masses;
    std::vector<double> planetRadii;
    std::vector<double> orbitRadii;
    std::vector<PhysicsObject> physicsObjects;

    void InitSolarSystem();
    void InitRandomSystem(unsigned int numPlanets);

    void UpdatePhysicsObjects(float timePassed);
    void UpdatePositions(float timePassed);

    Vec3 GetRelativePosn(unsigned int planet);
    Vec3 GetPlanetAccel(unsigned int planet);
    double GetPlanetGravForce(unsigned int p1, unsigned int p2, double radius);
    double GetPlanetVolume(unsigned int planet);
};
