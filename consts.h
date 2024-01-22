#pragma once

#include "raylib.h"
#include <array>

// astronomic units scale, where 1.0f = 1km
constexpr float AU = 1.5e8f;

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

constexpr std::array<float, numPlanets> planetRadii = { // km
    695508.0f / 20.0f, // scale down
    2440.0f,
    6052.0f,
    6371.0f,
    3390.0f,
    69911.0f,
    58232.0f,
    25362.0f,
    24622.0f,
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

constexpr std::array<float, numPlanets> planetVelocities = { // km s-1
    0.0f,
    47.4f,
    35.0f,
    29.8f,
    24.1f,
    13.1f,
    9.7f,
    6.8f,
    5.4f,
};

constexpr float gravConstant = 6.6743e-17; // N km2 kg-2

constexpr float gravForce(unsigned int p1, unsigned int p2)
{
    double massesMult = planetMasses[p1] * planetMasses[p2];
    float orbitSquared = orbitRadii[p2] * orbitRadii[p2];
    float force = gravConstant * massesMult / orbitSquared;
    force *= 1.0e24f;
    force *= 1.0e24f;

    return force;
}

const std::array<float, numPlanets> planetForces = { // n km2 kg-2
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
