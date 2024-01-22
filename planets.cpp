#include "planets.h"

#include <raylib.h>
#include <raymath.h>

#include "consts.h"

// TODO double defined - bad
constexpr Vector3 UP_VECTOR = (Vector3){0.0f, 0.0f, 1.0f};

Planets::Planets()
{}

void Planets::Init(bool randomSystem /* = false */, unsigned int numPlanets /* = 0 */)
{
    if(randomSystem) InitRandomSystem(numPlanets);
    else InitSolarSystem();
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


        Vector3 initialPosn =   Vector3Scale(
                                    Vector3Normalize(
                                    (Vector3){  static_cast<float>(GetRandomValue(-100, 100)),
                                                static_cast<float>(GetRandomValue(-100, 100)),
                                                0.0f}
                                    ),
                                    orbitRadii[i]
                                );

        TraceLog(LOG_INFO, "%d", GetRandomValue(0, 100));
        TraceLog(LOG_INFO, "%f, %f", initialPosn.x, initialPosn.y);

        //initialPosn = (Vector3){orbitRadii[i], 0.0f,  0.0f};
        positions.push_back(initialPosn);
        
        radii.push_back(planetRadii[i]);

        Vector3 initialVel =    Vector3Scale(
                                    Vector3Normalize(
                                        Vector3CrossProduct(initialPosn,
                                                            UP_VECTOR)
                                        ),
                                    planetInitialVelocities[i]
                                );

        
        //initialVel = (Vector3){0.0f, planetInitialVelocities[i], 0.0f};
        PhysicsObject obj = {initialVel,
                             GetPlanetAccel(i)};

        physicsObjects.push_back(obj);
    }
}

void Planets::InitRandomSystem(unsigned int numPlanets)
{

}

void Planets::Update(float timePassed)
{
    UpdatePhysicsObjects(timePassed);
    UpdatePositions(timePassed);
}

void Planets::UpdatePhysicsObjects(float timePassed)
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        physicsObjects[i].acceleration = GetPlanetAccel(i);

        physicsObjects[i].velocity =Vector3Add(     physicsObjects[i].velocity,
                                    Vector3Scale(   physicsObjects[i].acceleration,
                                                    timePassed));
    }
}

void Planets::UpdatePositions(float timePassed)
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        positions[i] = Vector3Add(positions[i], Vector3Scale(physicsObjects[i].velocity, timePassed));
    }
}

void Planets::DrawPlanets()
{
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        DrawSphereWires(Vector3Scale(positions[i], 1.0f / AU), radii[i] * 1000.0f / AU, 7,8, planetColours[i]);
    }
}

Vector3 Planets::GetPlanetAccel(std::size_t planet)
{
    float accel = planetForces[planet] / (planetMasses[planet] * 1.0e24f);

    Vector3 dir =   Vector3Normalize(
                    Vector3Subtract(positions[parents[planet]],
                                    positions[planet]));

    return Vector3Scale(dir, accel);
}

