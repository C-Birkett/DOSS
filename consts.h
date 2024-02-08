#pragma once

#include "raylib.h"

#include <array>
#include <cmath>
#include <ranges>

//-----------------------------------------------------------------------------

// vector space consts
constexpr Vector3 ORIGIN = {0.0f, 0.0f, 0.0f};
constexpr Vector3 UP_VECTOR = {0.0f, 0.0f, 1.0f};

// astronomic units scale, where 1.0f = 1m
constexpr float AU = 1.5e11f;

// gravitational constant G
constexpr double GRAV_CONSTANT = 6.6743e-11; // N m2 kg-2

// store mass values as 10^24 kg
constexpr float MASS_ADJUST = 1.0e24f;

//-----------------------------------------------------------------------------

// gravitational force = Gm1m2/r^2
template <class T>
requires std::ranges::contiguous_range<T> // TODO stricter reqs
constexpr float GravForce(unsigned int p1, unsigned int p2, const T& masses, const T& radii)
{
    double massesMult = masses[p1] * masses[p2];
    double orbitSquared = radii[p2] * radii[p2];
    double force = GRAV_CONSTANT * massesMult / orbitSquared;

    // adjust for mass
    force *= MASS_ADJUST;
    force *= MASS_ADJUST;

    return static_cast<float>(force);
}

//-----------------------------------------------------------------------------

// orbit velocity
// dodgy assumption that mass of parent >> mass of child
template <class T, class U>
requires std::ranges::contiguous_range<T> && std::ranges::contiguous_range<U> // TODO stricter reqs
constexpr float InitialVelocity(unsigned int p, const T& masses, const T& radii, const U& parents)
{
    float Gm = static_cast<float>(GRAV_CONSTANT * masses[parents[p]] * MASS_ADJUST);
    return sqrtf(Gm / radii[p]); // NB - sqrtf not normally constexpr but gcc allows it
}

//-----------------------------------------------------------------------------

namespace SolarSystem
{
    enum planets : unsigned char
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

    constexpr std::array<unsigned int, numPlanets> planetParents{ 0 };

    constexpr std::array<float, numPlanets> orbitRadii
    { // AU
        0.0f,
        0.3871f * AU,
        0.7233f * AU,
        1.0f * AU,
        1.5273f * AU,
        5.2028f * AU,
        9.5388f * AU,
        19.1914f * AU,
        30.0611f * AU,
    };

    constexpr std::array<float, numPlanets> planetRadii
    { // m
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

    // * by MASS_ADJUST for mass in kg
    constexpr std::array<float, numPlanets> planetMasses
    { // e24 kg
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

    constexpr std::array<float, numPlanets> planetForces
    { // N
        0.0f,
        GravForce(sun, mercury, planetMasses, orbitRadii),
        GravForce(sun, venus, planetMasses, orbitRadii),
        GravForce(sun, earth, planetMasses, orbitRadii),
        GravForce(sun, mars, planetMasses, orbitRadii),
        GravForce(sun, jupiter, planetMasses, orbitRadii),
        GravForce(sun, saturn, planetMasses, orbitRadii),
        GravForce(sun, uranus, planetMasses, orbitRadii),
        GravForce(sun, neptune, planetMasses, orbitRadii),
    };

    constexpr std::array<float, numPlanets> planetInitialVelocities
    { // m s-1
        0.0f,
        InitialVelocity(mercury, planetMasses, orbitRadii, planetParents),
        InitialVelocity(venus, planetMasses, orbitRadii, planetParents),
        InitialVelocity(earth, planetMasses, orbitRadii, planetParents),
        InitialVelocity(mars, planetMasses, orbitRadii, planetParents),
        InitialVelocity(jupiter, planetMasses, orbitRadii, planetParents),
        InitialVelocity(saturn, planetMasses, orbitRadii, planetParents),
        InitialVelocity(uranus, planetMasses, orbitRadii, planetParents),
        InitialVelocity(neptune, planetMasses, orbitRadii, planetParents)
    };

    constexpr std::array<Color, numPlanets> planetColours
    {
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
}
