#pragma once

#include "vec.h"

#include <vector>
#include <span>

namespace planets
{

constexpr float innerRadius = 1.5f;
constexpr float outerRaidus = 5.5f;
constexpr float pixelToMeter = 100.f;

struct Planet
{
    Vec2f position{};
    Vec2f velocity{};
};
class PlanetSystem
{
public:
    PlanetSystem(std::size_t planetCount);
    void Update(float dt);
    std::span<Planet> GetPlanets() { return planets_;}
private:
    std::vector<Planet> planets_;
};



class PlanetSystem4
{
public:
    PlanetSystem4(std::size_t planetCount);
    void Update(float dt);
    std::span<Planet> GetPlanets() { return planets_;}
private:
    std::vector<Planet> planets_;
    std::vector<FourVec2f> positions_;
    std::vector<FourVec2f> velocities_;
};

}