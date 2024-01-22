#include "planets.h"

#include <raylib.h>
#include <raymath.h>

#include "consts.h"

Planets::Planets()
{
    InitSolarSystem();
}

Vector3 Planets::GetPlanetAccel(size_t planet)
{
    float accel = planetForces[planet] / planetMasses[planet];
    return Vector3Scale(Vector3Normalize(Vector3Negate(positions[planet])), accel);
}

void Planets::InitSolarSystem()
{
    parents.reserve(numPlanets);
    positions.reserve(numPlanets);
    radii.reserve(numPlanets);
    physicsObjects.reserve(numPlanets);

    for(std::size_t i = 0; i < numPlanets; i++)
    {
        parents.push_back(planetParents[i]);

        positions.push_back((Vector3){orbitRadii[i], 0.0f, 0.0f});
        
        radii.push_back(planetRadii[i]);

        PhysicsObject obj = {(Vector3){0.0f, planetVelocities[i], 0.0f},
                              GetPlanetAccel(i)};

        physicsObjects.push_back(obj);
    }

    parents[1] = 3;
}

void Planets::InitRandomSystem(size_t numPlanets)
{

}

void Planets::UpdatePhysicsObjects(float timePassed)
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        physicsObjects[i].acceleration = GetPlanetAccel(i);
        physicsObjects[i].velocity = Vector3Add(physicsObjects[i].velocity, Vector3Scale(physicsObjects[i].acceleration, timePassed));
    }
}

void Planets::UpdatePositions(float timePassed)
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        physicsObjects[i].velocity = Vector3Scale(Vector3Normalize(Vector3CrossProduct(Vector3Subtract(positions[i], positions[parents[i]]), (Vector3){0.0,0.0,1.0})), planetVelocities[i]);

        positions[i] = Vector3Add(positions[i], Vector3Scale(physicsObjects[i].velocity, timePassed));
    }
}

void Planets::DrawPlanets()
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        //TraceLog(LOG_INFO, "planet: %d position: %f radii: %f", i, positions[i].x / AU, radii[i] / AU);
        DrawSphereWires(Vector3Scale(positions[i], 1.0f / AU), radii[i] * 1000.0f / AU, 7,8, planetColours[i]);
        //DrawSphere(Vector3Scale(positions[i], 1.0f / AU), radii[i] * 1000.0f / AU, planetColours[i]);
    }
}
