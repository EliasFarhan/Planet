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
    std::uniform_real_distribution<float> disRadius(innerRadius, outerRaidus);
    std::uniform_real_distribution<float> disAngle(0, pi);

    for(std::size_t i = 0; i < planetCount; i++)
    {
        const auto radius = disRadius(gen);
        const auto angle = disAngle(gen);

        Planet planet;

        const auto v = Vec2f::up().Rotate(angle) * radius;

        planet.position = v + worldCenter;
        planet.velocity = (planet.position-worldCenter).Perpendicular().Normalized() * std::sqrt(CalculateAcceleration(radius) * radius);
        planets_.push_back(planet);

    }
}

void PlanetSystem::Update(float dt)
{
    for (auto& planet : planets_)
    {
        //Calculate new velocity
        const auto radius = (planet.position-worldCenter).Magnitude();
        const auto accelerationValue = CalculateAcceleration(radius);
        const auto acceleration = (worldCenter-planet.position).Normalized()*accelerationValue;
        planet.velocity += acceleration * dt;
        //Calculate new position
        planet.position += planet.velocity * dt;
    }
}

PlanetSystem4::PlanetSystem4(std::size_t planetCount)
{
    planets_.reserve(planetCount);
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> disRadius(innerRadius, outerRaidus);
    std::uniform_real_distribution<float> disAngle(0, pi);

    for (std::size_t i = 0; i < planetCount; i++)
    {
        const auto radius = disRadius(gen);
        const auto angle = disAngle(gen);

        Planet planet;

        const auto v = Vec2f::up().Rotate(angle) * radius;

        planet.position = v + worldCenter;
        planet.velocity = (planet.position - worldCenter).Perpendicular().Normalized() * std::sqrt(CalculateAcceleration(radius) * radius);
        planets_.push_back(planet);

    }
    positions_.resize(planetCount / 4 + 1);
    velocities_.resize(planetCount / 4 + 1);
    for(std::size_t i = 0; i < planetCount/4+1; i++)
    {
        const std::array<Vec2f, 4> posTmp = {
            {
                i*4<planetCount?planets_[i * 4].position:defaultPos,
                i*4+1<planetCount?planets_[i * 4 + 1].position : defaultPos,
                i*4+2<planetCount?planets_[i * 4 + 2].position : defaultPos,
                i*4+3<planetCount?planets_[i * 4 + 3].position : defaultPos
            } };
        positions_[i] = FourVec2f{ posTmp };
        const std::array<Vec2f, 4> velTmp = {
    {
        i * 4 < planetCount ? planets_[i * 4].velocity : defaultVel,
        i * 4 + 1 < planetCount ? planets_[i * 4 + 1].velocity : defaultVel,
        i * 4 + 2 < planetCount ? planets_[i * 4 + 2].velocity : defaultVel,
        i * 4 + 3 < planetCount ? planets_[i * 4 + 3].velocity : defaultVel
    } };
        velocities_[i] = FourVec2f{ velTmp };
    }
}

void PlanetSystem4::Update(float dt)
{
    for (std::size_t i = 0; i < velocities_.size(); i++)
    {
        //Calculate new velocity
        const auto fourWorldCenter = FourVec2f{ worldCenter };
        const auto radius = (positions_[i] - fourWorldCenter).Magnitude();
        const auto accelerationValue = CalculateAcceleration(radius);
        const auto acceleration = (fourWorldCenter - positions_[i]).Normalized() * accelerationValue;
        const auto fourDt = FourFloat{dt};
        velocities_[i] += acceleration * fourDt;
        //Calculate new position
        positions_ [i] += velocities_[i] * fourDt;
    }

    for(std::size_t i = 0; i < planets_.size(); i++)
    {
        planets_[i].position = { positions_[i / 4].Xs()[i % 4], positions_[i / 4].Ys()[i % 4] };
    }
}

PlanetSystem8::PlanetSystem8(std::size_t planetCount)
{
    planets_.reserve(planetCount);
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> disRadius(innerRadius, outerRaidus);
    std::uniform_real_distribution<float> disAngle(0, pi);

    for (std::size_t i = 0; i < planetCount; i++)
    {
        const auto radius = disRadius(gen);
        const auto angle = disAngle(gen);

        Planet planet;

        const auto v = Vec2f::up().Rotate(angle) * radius;

        planet.position = v + worldCenter;
        planet.velocity = (planet.position - worldCenter).Perpendicular().Normalized() * std::sqrt(CalculateAcceleration(radius) * radius);
        planets_.push_back(planet);

    }
    positions_.resize(planetCount / 8 + 1);
    velocities_.resize(planetCount / 8 + 1);
    for(std::size_t i = 0; i < planetCount/8+1; i++)
    {
        const std::array<Vec2f, 8> posTmp = {
                {
                        i*8<planetCount?planets_[i * 8].position:defaultPos,
                        i*8+1<planetCount?planets_[i * 8 + 1].position : defaultPos,
                        i*8+2<planetCount?planets_[i * 8 + 2].position : defaultPos,
                        i*8+3<planetCount?planets_[i * 8 + 3].position : defaultPos,
                        i*8+4<planetCount?planets_[i * 8 + 4].position : defaultPos,
                        i*8+5<planetCount?planets_[i * 8 + 5].position : defaultPos,
                        i*8+6<planetCount?planets_[i * 8 + 6].position : defaultPos,
                        i*8+7<planetCount?planets_[i * 8 + 7].position : defaultPos,
                } };
        positions_[i] = EightVec2f{ posTmp };
        const std::array<Vec2f, 8> velTmp = {
                {
                        i * 8 < planetCount ? planets_[i * 8].velocity : defaultVel,
                        i * 8 + 1 < planetCount ? planets_[i * 8 + 1].velocity : defaultVel,
                        i * 8 + 2 < planetCount ? planets_[i * 8 + 2].velocity : defaultVel,
                        i * 8 + 3 < planetCount ? planets_[i * 8 + 3].velocity : defaultVel,
                        i * 8 + 4 < planetCount ? planets_[i * 8 + 4].velocity : defaultVel,
                        i * 8 + 5 < planetCount ? planets_[i * 8 + 5].velocity : defaultVel,
                        i * 8 + 6 < planetCount ? planets_[i * 8 + 6].velocity : defaultVel,
                        i * 8 + 7 < planetCount ? planets_[i * 8 + 7].velocity : defaultVel,
                } };
        velocities_[i] = EightVec2f{ velTmp };
    }
}

void PlanetSystem8::Update(float dt)
{
    for (std::size_t i = 0; i < velocities_.size(); i++)
    {
        //Calculate new velocity
        const auto eightWorldCenter = EightVec2f {worldCenter };
        const auto radius = (positions_[i] - eightWorldCenter).Magnitude();
        const auto accelerationValue = CalculateAcceleration(radius);
        const auto acceleration = (eightWorldCenter - positions_[i]).Normalized() * accelerationValue;
        const auto eightDt = EightFloat{dt};
        velocities_[i] += acceleration * eightDt;
        //Calculate new position
        positions_ [i] += velocities_[i] * eightDt;
    }

    for(std::size_t i = 0; i < planets_.size(); i++)
    {
        planets_[i].position = { positions_[i / 8].Xs()[i % 8], positions_[i / 8].Ys()[i % 8] };
    }
}
}
