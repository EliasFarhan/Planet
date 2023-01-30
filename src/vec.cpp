//
// Created by efarhan on 1/27/23.
//

#include "vec.h"

namespace planets
{
#ifdef USE_INTRINSICS


#if defined(__SSE__)
template<>
FourFloat FourFloat::Sqrt() const
{
    auto vs = _mm_loadu_ps(data());
    vs = _mm_sqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(result.data(), vs);
    return result;
}

template<>
FourFloat FourFloat::ReciprocalSqrt() const
{
    auto vs = _mm_loadu_ps(data());
    vs = _mm_rsqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(result.data(), vs);
    return result;
}

template<>
FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const
{
    auto v1s = _mm_loadu_ps(data());
    auto v2s = _mm_loadu_ps(rhs.data());
    v1s = _mm_mul_ps(v1s, v2s);

    FourFloat result;
    _mm_storeu_ps(result.data(), v1s);
    return result;
}

template<>
FourFloat FourFloat::operator*(float rhs) const
{
    auto v1s = _mm_loadu_ps(data());
    auto v2 = _mm_load1_ps(&rhs);
    v1s = _mm_mul_ps(v1s, v2);

    FourFloat result;
    _mm_storeu_ps(result.data(), v1s);
    return result;
}

template<>
FourVec2f FourVec2f::operator+(const FourVec2f& v) const
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
FourVec2f FourVec2f::operator-(const FourVec2f& v) const
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
FourVec2f FourVec2f::operator*(const FloatArray<4>& ns) const
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
FourVec2f FourVec2f::operator/(const FloatArray<4>& ns) const
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
FourFloat FourVec2f::Dot(const NVec2f<4> &v1, const NVec2f<4> &v2)
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

#endif
}