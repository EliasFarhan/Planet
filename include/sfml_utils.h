#pragma once

#include "vec.h"

#include <SFML/System/Vector2.hpp>

namespace planets
{

inline sf::Vector2f ToSf(Vec2f v) noexcept
{
    return { v.x, v.y };
}

}
