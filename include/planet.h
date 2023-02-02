#pragma once

#include "vec.h"

#include <vector>
#include <span>

namespace planets
{

constexpr float innerRadius = 1.5f;
constexpr float outerRaidus = 5.5f;
constexpr float pixelToMeter = 100.f;
constexpr float G = 100.0f;
constexpr Vec2f worldCenter{ 5.0f, 2.5f };
//Shoudl not be equal to worldCenter
constexpr Vec2f defaultPos{ 1.0f, 1.0f };
constexpr Vec2f defaultVel{ 100.0f, 100.0f };

struct Planet
{
    Vec2f position{};
    Vec2f velocity{};
};

constexpr float CalculateAcceleration(float radius) noexcept
{
    return G / (radius * radius);
}

inline FourFloat CalculateAcceleration(const FourFloat& radius) noexcept
{
    const FourFloat g{ G };
    return g / (radius * radius);
}

inline EightFloat CalculateAcceleration(const EightFloat& radius) noexcept
{
    const EightFloat g{ G };
    return g / (radius * radius);
}



class PlanetSystem
{
public:
    PlanetSystem(std::size_t planetCount) noexcept;
    void Update(float dt) noexcept;
    std::span<Planet> GetPlanets() noexcept { return planets_;}
    Vec2f GetPosition(int index) const;

private:
    std::vector<Planet> planets_;
};


class PlanetSystem4
{
public:
    PlanetSystem4(std::size_t planetCount) noexcept;
    void Update(float dt) noexcept;
    Vec2f GetPosition(int index) const;
private:
    std::vector<FourVec2f> positions_;
    std::vector<FourVec2f> velocities_;
};

class PlanetSystem8
{
public:
    PlanetSystem8(std::size_t planetCount) noexcept;
    void Update(float dt) noexcept;
    Vec2f GetPosition(int index) const;
private:
    std::vector<EightVec2f> positions_;
    std::vector<EightVec2f> velocities_;
};
}