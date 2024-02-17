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
constexpr double AU = 1.5e11;

// gravitational constant G
constexpr double GRAV_CONSTANT = 6.6743e-11; // N m2 kg-2

// store mass values as 10^24 kg
constexpr double MASS_ADJUST = 1.0e24;

// draw sun 20x smaller than actual size
constexpr double SUN_SCALE = 1.0 / 20.0;

//-----------------------------------------------------------------------------

// gravitational force = Gm1m2/r^2
template <class T>
requires std::ranges::contiguous_range<T> // TODO stricter reqs
constexpr double GravForce(unsigned int p1, unsigned int p2, const T& masses, const T& radii)
{
    double massesMult = masses[p1] * masses[p2];
    double orbitSquared = radii[p2] * radii[p2];
    double force = GRAV_CONSTANT * massesMult / orbitSquared;

    // adjust for mass
    force *= MASS_ADJUST;
    force *= MASS_ADJUST;

    return force;
}

//-----------------------------------------------------------------------------

// orbit velocity
// dodgy assumption that mass of parent >> mass of child
template <class T, class U>
requires std::ranges::contiguous_range<T> && std::ranges::contiguous_range<U> // TODO stricter reqs
constexpr double InitialVelocity(unsigned int p, const T& masses, const T& radii, const U& parents)
{
    double Gm = GRAV_CONSTANT * masses[parents[p]] * MASS_ADJUST;
    return sqrt(Gm / radii[p]); // NB - sqrtf not normally constexpr but gcc allows it
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

    constexpr std::array<double, numPlanets> orbitRadii
    { // AU
        0.0,
        0.3871 * AU,
        0.7233 * AU,
        1.0 * AU,
        1.5273 * AU,
        5.2028 * AU,
        9.5388 * AU,
        19.1914 * AU,
        30.0611 * AU,
    };

    constexpr std::array<double, numPlanets> planetRadii
    { // m
        695508.0e3 * SUN_SCALE,
        2440.0e3,
        6052.0e3,
        6371.0e3,
        3390.0e3,
        69911.0e3,
        58232.0e3,
        25362.0e3,
        24622.0e3,
    };

    // * by MASS_ADJUST for mass in kg
    constexpr std::array<double, numPlanets> planetMasses
    { // e24 kg
        1.989e6,
        0.330,
        4.87,
        5.97,
        0.642,
        1898.0,
        568.0,
        86.8,
        102.0,
    };

    constexpr std::array<double, numPlanets> planetForces
    { // N
        0.0,
        GravForce(sun, mercury, planetMasses, orbitRadii),
        GravForce(sun, venus, planetMasses, orbitRadii),
        GravForce(sun, earth, planetMasses, orbitRadii),
        GravForce(sun, mars, planetMasses, orbitRadii),
        GravForce(sun, jupiter, planetMasses, orbitRadii),
        GravForce(sun, saturn, planetMasses, orbitRadii),
        GravForce(sun, uranus, planetMasses, orbitRadii),
        GravForce(sun, neptune, planetMasses, orbitRadii),
    };

    constexpr std::array<double, numPlanets> planetInitialVelocities
    { // m s-1
        0.0,
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
