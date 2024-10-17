#pragma once

#include "intrinsics.h"
#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <array>
#include <random>

namespace planets
{

struct Vec2f
{
    float x=0.0f, y=0.0f;

    constexpr static Vec2f zero() noexcept {return {};}
    constexpr static Vec2f one() noexcept {return {1.0f, 1.0f};}
    constexpr static Vec2f up() noexcept {return {0.0f, 1.0f};}
    constexpr static Vec2f down() noexcept {return {0.0f, -1.0f};}
    constexpr static Vec2f left() noexcept {return {-1.0f, 0.0f};}
    constexpr static Vec2f right() noexcept {return {1.0f, 0.0f};}

    constexpr Vec2f operator+(Vec2f other) const  noexcept { return {x+other.x, y+other.y}; }
    constexpr Vec2f& operator+=(Vec2f other) noexcept
    {
        x += other.x;
        y+= other.y;
        return *this;
    }
    constexpr Vec2f operator-() const noexcept { return { -x, -y }; }
    constexpr Vec2f operator-(Vec2f other) const noexcept { return {x-other.x, y-other.y}; }
    constexpr Vec2f& operator-=(Vec2f other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    constexpr Vec2f operator*(float f) const noexcept {return {x*f, y*f};}
    constexpr Vec2f operator/(float f) const noexcept {return {x/f, y/f};}
    [[nodiscard]] Vec2f Rotate(float angle) const noexcept
    {
        const float sin = std::sin(angle);
        const float cos = std::cos(angle);
        return { (cos * x) - (sin * y) , (sin * x) + (cos * y) };
    }

    [[nodiscard]] constexpr Vec2f Perpendicular() const noexcept
    {
        return { y, -x };
    }

    constexpr static float Dot(Vec2f v1, Vec2f v2) noexcept
    {
        return v1.x*v2.x+v1.y*v2.y;
    }

    constexpr static float Det(Vec2f v1, Vec2f v2) noexcept
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    [[nodiscard]] constexpr float SquareMagnitude() const noexcept
    {
        return Dot(*this, *this);
    }

    [[nodiscard]] float Magnitude() const noexcept
    {
        return std::sqrt(SquareMagnitude());
    }

    [[nodiscard]] Vec2f Normalized() const noexcept
    {
        return *this/Magnitude();
    }

    static float AngleBetween(Vec2f v1, Vec2f v2) noexcept
    {
        const auto dot = Dot(v1, v2);
        const auto det = Det(v1, v2);
        return  std::atan2(det, dot);
    }

    explicit operator sf::Vector2f() const noexcept
    {
        return { x, y };
    }
};


constexpr Vec2f operator*(float f, Vec2f v) noexcept
{
    return v*f;
}

template<int N>
class FloatArray
{
public:
    FloatArray() = default;
    explicit FloatArray(float f) noexcept;
    explicit FloatArray(const float* ptr) noexcept;
    const float& operator[](int i) const  noexcept { return ns_[i]; }
    float& operator[](int i) noexcept { return ns_[i]; }

    [[nodiscard]] const float* data() const noexcept { return ns_.data(); }
    float* data() noexcept { return ns_.data(); }

    FloatArray<N> operator+(const FloatArray<N>& other) const noexcept;
    FloatArray<N> operator-(const FloatArray<N>& other) const noexcept;
    FloatArray<N> operator*(const FloatArray<N>& other) const noexcept;
    FloatArray<N> operator*(float f) const noexcept;
    FloatArray<N> operator/(const FloatArray<N>& other) const noexcept;
    FloatArray<N> operator/(float f) const noexcept;
    [[nodiscard]] FloatArray<N> Sqrt() const noexcept;
    [[nodiscard]] FloatArray<N> ReciprocalSqrt() const noexcept;
private:
    std::array<float, N> ns_{};
};

using FourFloat = FloatArray<4>;
using EightFloat = FloatArray<8>;

template<int N>
class NVec2f
{
public:
    NVec2f() = default;

    constexpr explicit NVec2f(const Vec2f& v) noexcept
    {
        for (int i = 0; i < N; i++)
        {
            xs_[i] = v.x;
            ys_[i] = v.y;
        }
    }

    constexpr explicit NVec2f(const Vec2f* ptr) noexcept
    {
        for(int i = 0; i < N; i++)
        {
            xs_[i] = ptr[i].x;
            ys_[i] = ptr[i].y;
        }
    }

    constexpr explicit NVec2f(const std::array<Vec2f, N> array) noexcept
    {
        for (int i = 0; i < N; i++)
        {
            xs_[i] = array[i].x;
            ys_[i] = array[i].y;
        }
    }

    NVec2f<N> operator+(const NVec2f<N>& other) const noexcept;

    NVec2f<N>& operator+=(const NVec2f<N>& other) noexcept;

    NVec2f<N> operator-(const NVec2f<N>& other) const noexcept;
    NVec2f<N> operator*(const FloatArray<N>& ns) const noexcept;
    NVec2f<N> operator/(const FloatArray<N>& ns) const noexcept;

    static FloatArray<N> Dot(const NVec2f<N>& v1, const NVec2f<N>& v2) noexcept;

    static FloatArray<N> Det(const NVec2f<N>& v1, const NVec2f<N>& v2) noexcept;

    [[nodiscard]] FloatArray<N> SquareMagnitude() const noexcept
    {
        return Dot(*this, *this);
    }

    [[nodiscard]] FloatArray<N> Magnitude() const noexcept
    {
        return SquareMagnitude().Sqrt();
    }

    [[nodiscard]] NVec2f<N> Normalized() const noexcept
    {
        return (*this) * SquareMagnitude().ReciprocalSqrt();
    }

    [[nodiscard]] const auto& Xs() const noexcept {return xs_;}
    [[nodiscard]] const auto& Ys() const noexcept {return ys_;}

private:
    std::array<float, N> xs_{};
    std::array<float, N> ys_{};
};

using FourVec2f = NVec2f<4>;
using EightVec2f = NVec2f<8>;

#ifdef USE_INTRINSICS

#ifdef USE_INTRINSICS


#if defined(__SSE__)

template<>
FourFloat::FloatArray(float f) noexcept;

template<>
FourFloat::FloatArray(const float* f) noexcept;

template<>
FourFloat FourFloat::Sqrt() const noexcept;

template<>
FourFloat FourFloat::ReciprocalSqrt() const noexcept;

template<>
FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const noexcept;

template<>
FourFloat FourFloat::operator*(float rhs) const noexcept;

template<>
NVec2f<4> FourVec2f::operator+(const FourVec2f& v) const noexcept;

template<>
FourVec2f& FourVec2f::operator+=(const FourVec2f& v) noexcept;

template<>
FourVec2f FourVec2f::operator-(const FourVec2f& v) const noexcept;

template<>
FourVec2f FourVec2f::operator*(const FourFloat& ns) const noexcept;

template<>
FourVec2f FourVec2f::operator/(const FloatArray<4>& ns) const noexcept;

template<>
FourFloat FourVec2f::Dot(const NVec2f<4>& v1, const NVec2f<4>& v2) noexcept;
#endif


#ifdef __AVX2__

template<>
EightFloat::FloatArray(float f) noexcept;

template<>
EightFloat::FloatArray(const float* f) noexcept;

template<>
EightFloat EightFloat::Sqrt() const noexcept;

template<>
EightFloat EightFloat::ReciprocalSqrt() const noexcept;

template<>
EightFloat EightFloat::operator*(const EightFloat& rhs) const noexcept;

template<>
EightFloat EightFloat::operator*(float rhs) const noexcept;
//EightVec2f
template<>
EightVec2f EightVec2f::operator+(const EightVec2f& other) const noexcept;

template<>
EightVec2f& EightVec2f::operator+=(const EightVec2f& other) noexcept;

template<>
EightVec2f EightVec2f::operator-(const EightVec2f& other) const noexcept;

template<>
EightVec2f EightVec2f::operator*(const EightFloat& ns) const noexcept;

template<>
EightVec2f EightVec2f::operator/(const EightFloat& ns) const noexcept;

template<>
EightFloat EightVec2f::Dot(const EightVec2f& v1, const EightVec2f& v2) noexcept;
#endif

#endif

#endif

}