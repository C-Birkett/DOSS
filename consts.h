#pragma once

#include "raylib.h"
#include <array>
#include <cmath>

// vector consts
constexpr Vector3 ORIGIN = (Vector3){0.0f, 0.0f, 0.0f};
constexpr Vector3 UP_VECTOR = (Vector3){0.0f, 0.0f, 1.0f};

// astronomic units scale, where 1.0f = 1km
constexpr float AU = 1.5e11f;

// gravitational constant G
constexpr float gravConstant = 6.6743e-11; // N m2 kg-2

enum planets
{
    sun = 0,
    mercury,
    venus,
    earth,
    mars,
    jupiter,
    saturn,
    uranus,
    neptune,
    numPlanets,
};

const std::array<unsigned int, numPlanets> planetParents = {0}; // should default init to 0

constexpr std::array<float, numPlanets> orbitRadii = { // AU
    0.0f,
    0.3871f *AU,
    0.7233f *AU,
    1.0f    *AU,
    1.5273f *AU,
    5.2028f *AU,
    9.5388f *AU,
    19.1914f*AU,
    30.0611f*AU,
};

constexpr std::array<float, numPlanets> planetRadii = { // m
    695508.0e3f / 20.0f, // scale down
    2440.0e3f,
    6052.0e3f,
    6371.0e3f,
    3390.0e3f,
    69911.0e3f,
    58232.0e3f,
    25362.0e3f,
    24622.0e3f,
};

constexpr std::array<float, numPlanets> planetMasses = { // e24 kg
    1.989e6f,
    0.330f,
    4.87f,
    5.97f,
    0.642f,
    1898.0f,
    568.0f,
    86.8f,
    102.0f,
};

// gravitational force = Gm1m2/r^2
constexpr float gravForce(unsigned int p1, unsigned int p2)
{
    double massesMult = planetMasses[p1] * planetMasses[p2];
    double orbitSquared = orbitRadii[p2] * orbitRadii[p2];
    double force = (float)(gravConstant * massesMult / orbitSquared);

    // adjust for mass
    force *= 1.0e24f;
    force *= 1.0e24f;

    return (float)force;
}

const std::array<float, numPlanets> planetForces = { // N
    0.0f,
    gravForce(sun, mercury),
    gravForce(sun, venus),
    gravForce(sun, earth),
    gravForce(sun, mars),
    gravForce(sun, jupiter),
    gravForce(sun, saturn),
    gravForce(sun, uranus),
    gravForce(sun, neptune),
};

// orbit velocity
// dodgy assumption that mass of parent >> mass of child
constexpr float initialVelocity(unsigned int p)
{
    // possible overflow here!
    float Gm = gravConstant * planetMasses[planetParents[p]] * 1.0e24f;
    return sqrtf(Gm / orbitRadii[p]);
}

const std::array<float, numPlanets> planetInitialVelocities = { // m s-1
    0.0f,
    initialVelocity(mercury),
    initialVelocity(venus),
    initialVelocity(earth),
    initialVelocity(mars),
    initialVelocity(jupiter),
    initialVelocity(saturn),
    initialVelocity(uranus),
    initialVelocity(neptune)
};

constexpr std::array<Color, numPlanets> planetColours = {
    YELLOW,
    LIGHTGRAY,
    ORANGE,
    BLUE,
    RED,
    GOLD,
    BEIGE,
    SKYBLUE,
    DARKBLUE,
};
