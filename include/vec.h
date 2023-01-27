#pragma once

#include "intrinsics.h"

#include <cmath>
#include <array>

#define USE_INTRINSICS

namespace planets
{

struct Vec2f
{
    float x=0.0f, y=0.0f;

    constexpr static Vec2f zero(){return {};}
    constexpr static Vec2f one(){return {1.0f, 1.0f};}
    constexpr static Vec2f up(){return {0.0f, 1.0f};}
    constexpr static Vec2f down(){return {0.0f, -1.0f};}
    constexpr static Vec2f left(){return {-1.0f, 0.0f};}
    constexpr static Vec2f right(){return {1.0f, 0.0f};}

    constexpr Vec2f operator+(Vec2f other) const { return {x+other.x, y+other.y}; }
    constexpr Vec2f& operator+=(Vec2f other)
    {
        x += other.x;
        y+= other.y;
        return *this;
    }
    constexpr Vec2f operator-(Vec2f other) const { return {x-other.x, y-other.y}; }
    constexpr Vec2f& operator-=(Vec2f other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    constexpr Vec2f operator*(float f) const {return {x*f, y*f};}
    constexpr Vec2f operator/(float f) const{return {x/f, y/f};}

    constexpr static float Dot(Vec2f v1, Vec2f v2)
    {
        return v1.x*v2.x+v1.y*v2.y;
    }

    [[nodiscard]] constexpr float SquareMagnitude() const
    {
        return Dot(*this, *this);
    }

    [[nodiscard]] float Magnitude() const
    {
        return std::sqrt(SquareMagnitude());
    }

    [[nodiscard]] Vec2f Normalized() const
    {
        return *this/Magnitude();
    }
};

constexpr Vec2f operator*(float f, Vec2f v)
{
    return v*f;
}

template<int N>
class FloatArray
{
public:
    FloatArray() = default;
    constexpr FloatArray(const float* ptr)
    {
        for(int i = 0; i < 4; i++)
        {
            ns_[i] = ptr[i];
        }
    }
    const float& operator[](int i) const { return ns_[i]; }
    float& operator[](int i){ return ns_[i]; }

    const float* data() const{return &ns_[0];}
    float* data(){return &ns_[0];}

    FloatArray<N> operator+(const FloatArray<N>& other) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]+other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator-(const FloatArray<N>& other) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]-other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator*(const FloatArray<N>& other) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]*other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator*(float f) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]*f;
        }
        return result;
    }
    FloatArray<N> operator/(const FloatArray<N>& other) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]/other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator/(float f) const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]/f;
        }
        return result;
    }
    [[nodiscard]] FloatArray<N> Sqrt() const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(ns_[i]);
        }
        return result;
    }
    [[nodiscard]] FloatArray<N> ReciprocalSqrt() const
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = 1.0f/std::sqrt(ns_[i]);
        }
        return result;
    }
private:
    std::array<float, N> ns_{};
};

using FourFloat = FloatArray<4>;

template<int N>
class NVec2f
{
public:
    NVec2f() = default;
    constexpr NVec2f(const Vec2f* ptr)
    {
        for(int i = 0; i < N; i++)
        {
            xs_[i] = ptr[i].x;
            ys_[i] = ptr[i].y;
        }
    }

    NVec2f<N> operator+(const NVec2f<N>& other) const
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]+other.xs_[i];
            result.ys_[i] = ys_[i]+other.ys_[i];
        }
        return result;
    }
    NVec2f<N> operator-(const NVec2f<N>& other) const
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]-other.xs_[i];
            result.ys_[i] = ys_[i]-other.ys_[i];
        }
        return result;
    }
    NVec2f<N> operator*(const FloatArray<N>& ns) const
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]*ns[i];
            result.ys_[i] = ys_[i]*ns[i];
        }
        return result;
    }
    NVec2f<N> operator/(const FloatArray<N>& ns) const
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]/ns[i];
            result.ys_[i] = ys_[i]/ns[i];
        }
        return result;
    }

    static FloatArray<N> Dot(const NVec2f<N>& v1, const NVec2f<N>& v2)
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = v1.xs_[i]*v2.xs_[i]+v1.ys_[i]*v2.ys_[i];
        }
        return result;
    }
    [[nodiscard]] FloatArray<N> SquareMagnitude() const
    {
        return Dot(*this, *this);
    }

    [[nodiscard]] const auto& Xs() const {return xs_;}
    [[nodiscard]] const auto& Ys() const {return ys_;}

private:
    std::array<float, N> xs_{};
    std::array<float, N> ys_{};
};

using FourVec2f = NVec2f<4>;

#ifdef USE_INTRINSICS

#if defined(__SSE__)
    template<>
    FourFloat FourFloat::Sqrt() const;

    template<>
    FourFloat FourFloat::ReciprocalSqrt() const;

    template<>
    FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const;

    template<>
    FourFloat FourFloat::operator*(float rhs) const;

    template<>
    FourVec2f FourVec2f::operator+(const NVec2f<4>& other) const;

    template<>
    FourVec2f FourVec2f::operator-(const NVec2f<4>& other) const;

    template<>
    FourFloat FourVec2f::Dot(const NVec2f<4> &v1, const NVec2f<4> &v2);
#endif

#endif

}