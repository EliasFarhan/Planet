#include "planet.h"

#include <numbers>
#include <random>

namespace planets
{

constexpr auto pi = std::numbers::pi_v<float>;

PlanetSystem::PlanetSystem(std::size_t planetCount)
{
    planets_.reserve(planetCount);
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> disRadius(innerRadius, outerRaidus);
    std::uniform_real_distribution<> disAngle(0, pi);

    for(std::size_t i = 0; i < planetCount; i++)
    {
        const auto radius = disRadius(gen);
        const auto angle = disAngle(gen);


    }
}

void PlanetSystem::Update(float dt)
{
}

PlanetSystem4::PlanetSystem4(std::size_t planetCount)
{
}

void PlanetSystem4::Update(float dt)
{
}
}
