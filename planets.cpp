#include "planets.h"

#include <raylib.h>
#include <raymath.h>

#include "Vec3.h"
#include "consts.h"

constexpr float PLANET_DRAW_SCALE = 1000.0f / AU;

Planets::Planets()
{}

void Planets::Init(unsigned int numPlanets /* = 0 */)
{
    if(numPlanets) InitRandomSystem(numPlanets);
    else InitSolarSystem();
}

void Planets::InitSolarSystem()
{
    m_numPlanets = SolarSystem::numPlanets;

    parents.reserve(m_numPlanets);
    positions.reserve(m_numPlanets);
    planetRadii.reserve(m_numPlanets);
    orbitRadii.reserve(m_numPlanets);
    masses.reserve(m_numPlanets);
    physicsObjects.reserve(m_numPlanets);

    Vec3 initialPosn = (Vector3){ 0.0f, 0.0f, 0.0f };
    Vec3 initialVel = (Vector3){ 0.0f, 0.0f, 0.0f };
    PhysicsObject physObj;
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        parents.push_back(SolarSystem::planetParents[i]);

        orbitRadii.push_back(SolarSystem::orbitRadii[i]);

        initialPosn = (Vector3){static_cast<float>(GetRandomValue(-100, 100)),
                                static_cast<float>(GetRandomValue(-100, 100)),
                                0.0f};

        initialPosn = initialPosn.normalize();
        initialPosn = initialPosn * orbitRadii[i];

        positions.push_back(initialPosn);
        
        planetRadii.push_back(SolarSystem::planetRadii[i]);
        masses.push_back(SolarSystem::planetMasses[i]);

        initialVel = initialPosn.cross(UP_VECTOR);
        initialVel = initialVel.normalize();
        initialVel = initialVel * SolarSystem::planetInitialVelocities[i];

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
    planetRadii.reserve(numPlanets);
    orbitRadii.reserve(numPlanets);
    masses.reserve(numPlanets);
    physicsObjects.reserve(numPlanets);

    // vars
    double planetRadius = 0.0;
    double orbitRadius = 0.0;
    double force = 0.0;
    Vec3 initialPosn = (Vector3){ 0.0f, 0.0f, 0.0f };
    Vec3 relPosn = (Vector3){ 0.0f, 0.0f, 0.0f };
    Vec3 initialVel = (Vector3){ 0.0f, 0.0f, 0.0f };
    PhysicsObject physObj = {initialVel, initialVel};

    // add star
    parents.push_back(0);
    positions.push_back(initialPosn);
    planetRadii.push_back(SolarSystem::planetRadii[0]); // planet radius
    orbitRadii.push_back(orbitRadius);
    masses.push_back(SolarSystem::planetMasses[0]);
    physicsObjects.push_back(physObj);

    for(unsigned int i = 1; i < numPlanets; i++)
    {
        unsigned int parent = 0;
        
        // let at least 1/3rd orbit the star
        if(i > numPlanets/3)
            parent = GetRandomValue(0, i-1);

        parents.push_back(parent);

        // random orbit radius, max 1/20 parents orbit of its parent, else 10 AU
        if(parent)
        {
            double maxOrbit = orbitRadii[parent] / (20.0 * AU);
            orbitRadius = static_cast<double>(GetRandomValue(20, static_cast<int>(maxOrbit * 100.0)));
        }
        else
        {
            orbitRadius = static_cast<double>(GetRandomValue(50, 1000));
        }

        orbitRadius /= 100.0;
        orbitRadius *= AU;
        orbitRadii.push_back(orbitRadius);
        
        initialPosn = (Vector3){static_cast<float>(GetRandomValue(-100, 100)),
                                static_cast<float>(GetRandomValue(-100, 100)),
                                0.0f};

        initialPosn = initialPosn.normalize();
        initialPosn = initialPosn * orbitRadius;
        initialPosn = initialPosn + positions[parent];

        positions.push_back(initialPosn);

        // random planet radius, max half size of parent
        planetRadius = static_cast<double>(GetRandomValue(1e6, (int)planetRadii[parent] / 2));
        planetRadii.push_back(planetRadius);

        // assume density of water (1m^3 = 1e3 kg) for now, get mass
        masses.push_back(GetPlanetVolume(i) * 1e3 / MASS_ADJUST);

        // initial velocity
        initialVel = GetRelativePosn(i).cross(UP_VECTOR);
        initialVel = initialVel.normalize();
        if(GetRandomValue(0, 1)) initialVel = initialVel * -1.0f; // randomise orbit direction
        initialVel = initialVel * InitialVelocity(i, masses, orbitRadii, parents);

        // add parents vel which adds its parent etc...
        initialVel = initialVel + physicsObjects[parent].velocity;

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

        physicsObjects[i].velocity = physicsObjects[i].velocity + (physicsObjects[i].acceleration * timePassed);
    }
}

void Planets::UpdatePositions(float timePassed)
{
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        positions[i] = positions[i] + (physicsObjects[i].velocity * timePassed);
    }
}

void Planets::DrawPlanets()
{
    for(std::size_t i = 0; i < m_numPlanets; i++)
    {
        DrawSphereWires(positions[i] / AU, planetRadii[i] * PLANET_DRAW_SCALE, 7,8, SolarSystem::planetColours[i]);
    }
}

// relative to it's parent
Vec3 Planets::GetRelativePosn(unsigned int planet)
{
    return positions[planet] - positions[parents[planet]];
}

Vec3 Planets::GetPlanetAccel(unsigned int planet)
{
    Vec3 accelVec = (Vector3){ 0.0f, 0.0f, 0.0f };
    if(!planet) return accelVec;

    unsigned int parent = parents[planet];
    double force = GravForce(parent, planet, masses, orbitRadii);
    double accel = force / (masses[planet] * MASS_ADJUST);
    
    Vec3 dir = positions[parent] - positions[planet];
    dir = dir.normalize();
    accelVec = dir * accel;
 
    // add parents accel which adds its parent etc...
    if(parent) accelVec = accelVec + physicsObjects[parent].acceleration;

    return accelVec;
}

constexpr double fourThirdsPi = 4.0/3.0 * PI;
double Planets::GetPlanetVolume(unsigned int planet)
{
    return fourThirdsPi * planetRadii[planet] * planetRadii[planet] * planetRadii[planet];
}
