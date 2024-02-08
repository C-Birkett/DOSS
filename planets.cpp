#include "planets.h"

#include <algorithm>
#include <raylib.h>
#include <raymath.h>

#include "consts.h"

Planets::Planets()
{}

void Planets::Init(unsigned int numPlanets /* = 0 */)
{
    if(numPlanets) InitRandomSystem(numPlanets);
    else InitSolarSystem();
}

void Planets::InitSolarSystem()
{
    m_numPlanets = numPlanets;

    parents.reserve(numPlanets);
    positions.reserve(numPlanets);
    radii.reserve(numPlanets);
    forces.reserve(numPlanets);
    masses.reserve(numPlanets);
    physicsObjects.reserve(numPlanets);

    Vector3 initialPosn;
    Vector3 initialVel;
    PhysicsObject physObj;
    for(std::size_t i = 0; i < numPlanets; i++)
    {
        parents.push_back(planetParents[i]);


        initialPosn =   Vector3Scale(
                                    Vector3Normalize(
                                        { static_cast<float>(GetRandomValue(-100, 100)),
                                          static_cast<float>(GetRandomValue(-100, 100)),
                                          0.0f}
                                    ),
                                    orbitRadii[i]
                                );

        positions.push_back(initialPosn);
        
        radii.push_back(planetRadii[i]);
        forces.push_back(planetForces[i]);
        masses.push_back(planetMasses[i]);

        Vector3 initialVel =    Vector3Scale(
                                    Vector3Normalize(
                                        Vector3CrossProduct(initialPosn,
                                                            UP_VECTOR)
                                        ),
                                    planetInitialVelocities[i]
                                );

        physObj = {initialVel,
                             GetPlanetAccel(i)};

        physicsObjects.push_back(physObj);
    }
}

void Planets::InitRandomSystem(unsigned int numPlanets)
{
    m_numPlanets = numPlanets;

    // prealloc memory
    parents.reserve(numPlanets);
    positions.reserve(numPlanets);
    radii.reserve(numPlanets);
    forces.reserve(numPlanets);
    masses.reserve(numPlanets);
    physicsObjects.reserve(numPlanets);

    // vars
    float planetRadius;
    float orbitRadius;
    Vector3 initialPosn;
    Vector3 relPosn;
    Vector3 initialVel;
    PhysicsObject physObj;

    // add star
    parents.push_back(0);
    positions.push_back(ORIGIN);
    radii.push_back(planetRadii[0]); // planet radius
    physObj = {ORIGIN, ORIGIN};
    physicsObjects.push_back(physObj);

    for(unsigned int i = 1; i < numPlanets; i++)
    {
        // let at least 1/3rd orbit the star
        if(i < numPlanets/3)
            parents.push_back(0);
        else
            parents.push_back(GetRandomValue(0, i-1));

        // random orbit radius, max 1/3 parents orbit of its parent, else 20 AU
        if(parents[i])
        {
            orbitRadius = (float)GetRandomValue(5,
                                        (int)(Vector3Length(GetRelativePosn(parents[i]))
                                                / 0.003f*AU));
        }
        else
        {
            orbitRadius = (float)GetRandomValue(5, 2000);
        }

        orbitRadius *= 3.0f * AU;
        
        initialPosn =   Vector3Scale(
                            Vector3Normalize(
                                {static_cast<float>(GetRandomValue(-100, 100)),
                                 static_cast<float>(GetRandomValue(-100, 100)),
                                 0.0f}
                            ),
                            orbitRadius
                        );

        initialPosn = Vector3Add(positions[parents[i]], initialPosn);
        positions.push_back(initialPosn);

        // random planet radius, max half size of parent
        planetRadius = (float)GetRandomValue(1000, (int)(radii[parents[i]] / 2.0));
        radii.push_back(planetRadius);

        // assume density of water for now, get mass
        masses.push_back(GetPlanetVolume(i) / 1.0e21f);
        forces.push_back(GetPlanetGravForce(i, parents[i], orbitRadius));

        // initial velocity
        Vector3 initialVel =    Vector3Scale(
                                    Vector3Normalize(
                                        Vector3CrossProduct(initialPosn,
                                                            UP_VECTOR)
                                        ),
                                    0.0f//1.0f * AU //planetInitialVelocities[i]
                                );

        physObj = {initialVel,
                    GetPlanetAccel(i)};

        physicsObjects.push_back(physObj);
    }
}

void Planets::Update(float timePassed)
{
    UpdatePhysicsObjects(timePassed);
    UpdatePositions(timePassed);
}

void Planets::UpdatePhysicsObjects(float timePassed)
{
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        physicsObjects[i].acceleration = GetPlanetAccel(i);

        physicsObjects[i].velocity =Vector3Add(     physicsObjects[i].velocity,
                                    Vector3Scale(   physicsObjects[i].acceleration,
                                                    timePassed));
    }
}

void Planets::UpdatePositions(float timePassed)
{
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        positions[i] = Vector3Add(positions[i], Vector3Scale(physicsObjects[i].velocity, timePassed));
    }
}

void Planets::DrawPlanets()
{
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        DrawSphereWires(Vector3Scale(positions[i], 1.0f / AU), radii[i] * 1000.0f / AU, 7,8, planetColours[i]);
    }
}

// relative to it's parent
Vector3 Planets::GetRelativePosn(unsigned int planet)
{
    return Vector3Subtract(positions[planet], positions[parents[planet]]);
}

Vector3 Planets::GetPlanetAccel(unsigned int planet)
{
    float accel = forces[planet] / (masses[planet] * 1.0e24f);

    Vector3 dir =   Vector3Normalize(
                    Vector3Subtract(positions[parents[planet]],
                                    positions[planet]));

    return Vector3Scale(dir, accel);
}

float Planets::GetPlanetGravForce(unsigned int p1, unsigned int p2, float radius)
{
    double massesMult = masses[p1] * masses[p2];
    double orbitSquared = radius * radius;
    double force = (float)(gravConstant * massesMult / orbitSquared);

    // adjust for mass
    force *= 1.0e24f;
    force *= 1.0e24f;

    return (float)force;
}

static const float fourThirdsPi = 4.0f/3.0f * PI;
float Planets::GetPlanetVolume(unsigned int planet)
{
    return fourThirdsPi * radii[planet] * radii[planet] * radii[planet];
}
