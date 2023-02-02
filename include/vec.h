#pragma once

#include "intrinsics.h"
#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <array>
#include <random>

#define USE_INTRINSICS
#define INTRINSICS_CPP

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
    explicit FloatArray(float f) noexcept
    {
        for (int i = 0; i < N; i++)
        {
            ns_[i] = f;
        }
    }
    explicit FloatArray(const float* ptr) noexcept
    {
        for(int i = 0; i < N; i++)
        {
            ns_[i] = ptr[i];
        }
    }
    const float& operator[](int i) const  noexcept { return ns_[i]; }
    float& operator[](int i) noexcept { return ns_[i]; }

    [[nodiscard]] const float* data() const noexcept { return ns_.data(); }
    float* data() noexcept { return ns_.data(); }

    FloatArray<N> operator+(const FloatArray<N>& other) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]+other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator-(const FloatArray<N>& other) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]-other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator*(const FloatArray<N>& other) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]*other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator*(float f) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]*f;
        }
        return result;
    }
    FloatArray<N> operator/(const FloatArray<N>& other) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]/other.ns_[i];
        }
        return result;
    }
    FloatArray<N> operator/(float f) const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = ns_[i]/f;
        }
        return result;
    }
    [[nodiscard]] FloatArray<N> Sqrt() const noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(ns_[i]);
        }
        return result;
    }
    [[nodiscard]] FloatArray<N> ReciprocalSqrt() const noexcept
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

    NVec2f<N> operator+(const NVec2f<N>& other) const noexcept
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]+other.xs_[i];
            result.ys_[i] = ys_[i]+other.ys_[i];
        }
        return result;
    }

    NVec2f<N>& operator+=(const NVec2f<N>& other) noexcept
    {
        for (int i = 0; i < N; i++)
        {
            xs_[i] += other.xs_[i];
            ys_[i] += other.ys_[i];
        }
        return *this;
    }

    NVec2f<N> operator-(const NVec2f<N>& other) const noexcept
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]-other.xs_[i];
            result.ys_[i] = ys_[i]-other.ys_[i];
        }
        return result;
    }
    NVec2f<N> operator*(const FloatArray<N>& ns) const noexcept
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]*ns[i];
            result.ys_[i] = ys_[i]*ns[i];
        }
        return result;
    }
    NVec2f<N> operator/(const FloatArray<N>& ns) const noexcept
    {
        NVec2f<N> result;
        for(int i = 0; i < N; i++)
        {
            result.xs_[i] = xs_[i]/ns[i];
            result.ys_[i] = ys_[i]/ns[i];
        }
        return result;
    }

    static FloatArray<N> Dot(const NVec2f<N>& v1, const NVec2f<N>& v2) noexcept
    {
        FloatArray<N> result;
        for(int i = 0; i < N; i++)
        {
            result[i] = v1.xs_[i]*v2.xs_[i]+v1.ys_[i]*v2.ys_[i];
        }
        return result;
    }

    static FloatArray<N> Det(const NVec2f<N>& v1, const NVec2f<N>& v2) noexcept
    {
        FloatArray<N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs_[i] * v2.ys_[i] - v1.ys_[i] * v2.xs_[i];
        }
        return result;
    }

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

#if !defined(INTRINSICS_CPP)

#if defined(__SSE__)

template<>
inline FourFloat::FloatArray(float f) noexcept
{
    auto v2 = _mm_load1_ps(&f);
    _mm_storeu_ps(data(), v2);
}

template<>
inline FourFloat::FloatArray(const float* f) noexcept
{
    auto v2 = _mm_loadu_ps(f);
    _mm_storeu_ps(data(), v2);
}

template<>
inline FourFloat FourFloat::Sqrt() const noexcept
{
    auto vs = _mm_loadu_ps(data());
    vs = _mm_sqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(result.data(), vs);
    return result;
}

template<>
inline FourFloat FourFloat::ReciprocalSqrt() const noexcept
{
    auto vs = _mm_loadu_ps(data());
    vs = _mm_rsqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(result.data(), vs);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const noexcept
{
    auto v1s = _mm_loadu_ps(data());
    auto v2s = _mm_loadu_ps(rhs.data());
    v1s = _mm_mul_ps(v1s, v2s);

    FourFloat result;
    _mm_storeu_ps(result.data(), v1s);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(float rhs) const noexcept
{
    auto v1s = _mm_loadu_ps(data());
    auto v2 = _mm_load1_ps(&rhs);
    v1s = _mm_mul_ps(v1s, v2);

    FourFloat result;
    _mm_storeu_ps(result.data(), v1s);
    return result;
}

template<>
inline NVec2f<4> FourVec2f::operator+(const FourVec2f& v) const noexcept
{
    FourVec2f fv3f;
    auto x1 = _mm_loadu_ps(xs_.data());
    auto y1 = _mm_loadu_ps(ys_.data());

    const auto x2 = _mm_loadu_ps(v.xs_.data());
    const auto y2 = _mm_loadu_ps(v.ys_.data());

    x1 = _mm_add_ps(x1, x2);
    y1 = _mm_add_ps(y1, y2);

    _mm_storeu_ps(fv3f.xs_.data(), x1);
    _mm_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline FourVec2f& FourVec2f::operator+=(const FourVec2f& v) noexcept
{
    auto x1 = _mm_loadu_ps(xs_.data());
    auto y1 = _mm_loadu_ps(ys_.data());

    const auto x2 = _mm_loadu_ps(v.xs_.data());
    const auto y2 = _mm_loadu_ps(v.ys_.data());

    x1 = _mm_add_ps(x1, x2);
    y1 = _mm_add_ps(y1, y2);

    _mm_storeu_ps(xs_.data(), x1);
    _mm_storeu_ps(ys_.data(), y1);
    return *this;
}

template<>
inline FourVec2f FourVec2f::operator-(const FourVec2f& v) const noexcept
{
    FourVec2f fv3f;
    auto x1 = _mm_loadu_ps(xs_.data());
    auto y1 = _mm_loadu_ps(ys_.data());

    const auto x2 = _mm_loadu_ps(v.xs_.data());
    const auto y2 = _mm_loadu_ps(v.ys_.data());

    x1 = _mm_sub_ps(x1, x2);
    y1 = _mm_sub_ps(y1, y2);

    _mm_storeu_ps(fv3f.xs_.data(), x1);
    _mm_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline FourVec2f FourVec2f::operator*(const FourFloat& ns) const noexcept
{
    FourVec2f fv3f;
    auto x1 = _mm_loadu_ps(xs_.data());
    auto y1 = _mm_loadu_ps(ys_.data());

    const auto x2 = _mm_loadu_ps(ns.data());
    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, x2);

    _mm_storeu_ps(fv3f.xs_.data(), x1);
    _mm_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline FourVec2f FourVec2f::operator/(const FloatArray<4>& ns) const noexcept
{
    FourVec2f fv3f;
    auto x1 = _mm_loadu_ps(xs_.data());
    auto y1 = _mm_loadu_ps(ys_.data());

    const auto x2 = _mm_loadu_ps(ns.data());
    x1 = _mm_div_ps(x1, x2);
    y1 = _mm_div_ps(y1, x2);

    _mm_storeu_ps(fv3f.xs_.data(), x1);
    _mm_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline FourFloat FourVec2f::Dot(const NVec2f<4>& v1, const NVec2f<4>& v2) noexcept
{
    FourFloat result;
    auto x1 = _mm_loadu_ps(v1.Xs().data());
    auto y1 = _mm_loadu_ps(v1.Ys().data());

    auto x2 = _mm_loadu_ps(v2.Xs().data());
    auto y2 = _mm_loadu_ps(v2.Ys().data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);

    x1 = _mm_add_ps(x1, y1);

    _mm_storeu_ps(result.data(), x1);
    return result;
}
#endif


#ifdef __AVX2__

template<>
inline EightFloat::FloatArray(float f) noexcept
{
    auto reg = _mm256_broadcast_ss(&f);
    _mm256_storeu_ps(data(), reg);
}

template<>
inline EightFloat::FloatArray(const float* f) noexcept
{
    auto reg = _mm256_load_ps(f);
    _mm256_storeu_ps(data(), reg);
}

template<>
inline EightFloat EightFloat::Sqrt() const noexcept
{
    auto vs = _mm256_loadu_ps(data());
    vs = _mm256_sqrt_ps(vs);

    EightFloat result;
    _mm256_storeu_ps(result.data(), vs);
    return result;
}

template<>
inline EightFloat EightFloat::ReciprocalSqrt() const noexcept
{
    auto vs = _mm256_loadu_ps(data());
    vs = _mm256_rsqrt_ps(vs);

    EightFloat result;
    _mm256_storeu_ps(result.data(), vs);
    return result;
}

template<>
inline EightFloat EightFloat::operator*(const EightFloat& rhs) const noexcept
{
    auto v1s = _mm256_loadu_ps(data());
    auto v2s = _mm256_loadu_ps(rhs.data());
    v1s = _mm256_mul_ps(v1s, v2s);

    EightFloat result;
    _mm256_storeu_ps(result.data(), v1s);
    return result;
}

template<>
inline EightFloat EightFloat::operator*(float rhs) const noexcept
{
    auto v1s = _mm256_loadu_ps(data());
    auto v2s = _mm256_broadcast_ss(&rhs);
    v1s = _mm256_mul_ps(v1s, v2s);

    EightFloat result;
    _mm256_storeu_ps(result.data(), v1s);
    return result;
}

//EightVec2f
template<>
inline EightVec2f EightVec2f::operator+(const EightVec2f& other) const noexcept
{
    EightVec2f fv3f;
    auto x1 = _mm256_loadu_ps(xs_.data());
    auto y1 = _mm256_loadu_ps(ys_.data());

    const auto x2 = _mm256_loadu_ps(other.xs_.data());
    const auto y2 = _mm256_loadu_ps(other.ys_.data());

    x1 = _mm256_add_ps(x1, x2);
    y1 = _mm256_add_ps(y1, y2);

    _mm256_storeu_ps(fv3f.xs_.data(), x1);
    _mm256_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline EightVec2f& EightVec2f::operator+=(const EightVec2f& other) noexcept
{
    auto x1 = _mm256_loadu_ps(xs_.data());
    auto y1 = _mm256_loadu_ps(ys_.data());

    const auto x2 = _mm256_loadu_ps(other.xs_.data());
    const auto y2 = _mm256_loadu_ps(other.ys_.data());

    x1 = _mm256_add_ps(x1, x2);
    y1 = _mm256_add_ps(y1, y2);

    _mm256_storeu_ps(xs_.data(), x1);
    _mm256_storeu_ps(ys_.data(), y1);
    return *this;
}

template<>
inline EightVec2f EightVec2f::operator-(const EightVec2f& other) const noexcept
{
    EightVec2f fv3f;
    auto x1 = _mm256_loadu_ps(xs_.data());
    auto y1 = _mm256_loadu_ps(ys_.data());

    const auto x2 = _mm256_loadu_ps(other.xs_.data());
    const auto y2 = _mm256_loadu_ps(other.ys_.data());

    x1 = _mm256_sub_ps(x1, x2);
    y1 = _mm256_sub_ps(y1, y2);

    _mm256_storeu_ps(fv3f.xs_.data(), x1);
    _mm256_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline EightVec2f EightVec2f::operator*(const EightFloat& ns) const noexcept
{
    EightVec2f fv3f;
    auto x1 = _mm256_loadu_ps(xs_.data());
    auto y1 = _mm256_loadu_ps(ys_.data());

    const auto x2 = _mm256_loadu_ps(ns.data());
    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, x2);

    _mm256_storeu_ps(fv3f.xs_.data(), x1);
    _mm256_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline EightVec2f EightVec2f::operator/(const EightFloat& ns) const noexcept
{
    EightVec2f fv3f;
    auto x1 = _mm256_loadu_ps(xs_.data());
    auto y1 = _mm256_loadu_ps(ys_.data());

    const auto x2 = _mm256_loadu_ps(ns.data());
    x1 = _mm256_div_ps(x1, x2);
    y1 = _mm256_div_ps(y1, x2);

    _mm256_storeu_ps(fv3f.xs_.data(), x1);
    _mm256_storeu_ps(fv3f.ys_.data(), y1);
    return fv3f;
}

template<>
inline EightFloat EightVec2f::Dot(const EightVec2f& v1, const EightVec2f& v2) noexcept
{
    EightFloat result;
    auto x1 = _mm256_loadu_ps(v1.Xs().data());
    auto y1 = _mm256_loadu_ps(v1.Ys().data());

    auto x2 = _mm256_loadu_ps(v2.Xs().data());
    auto y2 = _mm256_loadu_ps(v2.Ys().data());

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);

    x1 = _mm256_add_ps(x1, y1);

    _mm256_storeu_ps(result.data(), x1);
    return result;
}
#endif

#else

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
FourFloat FourFloat::operator*(const FourFloat& rhs) const noexcept;

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
FourVec2f FourVec2f::operator/(const FourFloat& ns) const noexcept;

template<>
FourFloat FourVec2f::Dot(const FourVec2f& v1, const FourVec2f& v2) noexcept;
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