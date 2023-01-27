//
// Created by efarhan on 1/27/23.
//

#include "gtest/gtest.h"
#include "vec.h"

TEST(Vec2f, Const)
{
    constexpr auto v0 = planets::Vec2f::zero();
    EXPECT_FLOAT_EQ(v0.x, 0.0f);
    EXPECT_FLOAT_EQ(v0.y, 0.0f);

    constexpr auto v1 = planets::Vec2f::one();
    EXPECT_FLOAT_EQ(v1.x, 1.0f);
    EXPECT_FLOAT_EQ(v1.y, 1.0f);

    constexpr auto v_up = planets::Vec2f::up();
    EXPECT_FLOAT_EQ(v_up.x, 0.0f);
    EXPECT_FLOAT_EQ(v_up.y, 1.0f);

    constexpr auto v_down = planets::Vec2f::down();
    EXPECT_FLOAT_EQ(v_down.x, 0.0f);
    EXPECT_FLOAT_EQ(v_down.y, -1.0f);

    constexpr auto v_left = planets::Vec2f::left();
    EXPECT_FLOAT_EQ(v_left.x, -1.0f);
    EXPECT_FLOAT_EQ(v_left.y, 0.0f);

    constexpr auto v_right = planets::Vec2f::right();
    EXPECT_FLOAT_EQ(v_right.x, 1.0f);
    EXPECT_FLOAT_EQ(v_right.y, 0.0f);
}


TEST(Vec2f, Add)
{
    constexpr auto v1 = planets::Vec2f{3.3f, 4.0f};
    constexpr auto v2 = planets::Vec2f{-1.5f, -7.0f};
    constexpr auto result = v1+v2;
    EXPECT_FLOAT_EQ(v1.x+v2.x, result.x);
    EXPECT_FLOAT_EQ(v1.y+v2.y, result.y);
}

TEST(Vec2f, Sub)
{
    constexpr auto v1 = planets::Vec2f{3.3f, 4.0f};
    constexpr auto v2 = planets::Vec2f{-1.5f, -7.0f};
    constexpr auto result = v1-v2;
    EXPECT_FLOAT_EQ(v1.x-v2.x, result.x);
    EXPECT_FLOAT_EQ(v1.y-v2.y, result.y);
}

TEST(Vec2f, MulFLoat)
{
    constexpr auto v1 = planets::Vec2f{3.0f, 4.0f};
    constexpr auto f = -7.4f;
    constexpr auto result = v1 * f;
    EXPECT_FLOAT_EQ(v1.x * f, result.x);
    EXPECT_FLOAT_EQ(v1.y * f, result.y);

    constexpr auto result1 = f * v1;

    EXPECT_FLOAT_EQ(v1.x * f, result1.x);
    EXPECT_FLOAT_EQ(v1.y * f, result1.y);
}

TEST(Vec2f, DivFLoat)
{
    constexpr auto v1 = planets::Vec2f{3.5f, 4.0f};
    constexpr auto f = -7.4f;
    constexpr auto result = v1 / f;
    EXPECT_FLOAT_EQ(v1.x / f, result.x);
    EXPECT_FLOAT_EQ(v1.y / f, result.y);
}

TEST(Vec2f, Dot)
{
    constexpr auto v1 = planets::Vec2f{3.0f, 4.0f};
    constexpr auto v2 = planets::Vec2f{-1.5f, -7.0f};
    constexpr auto result = planets::Vec2f::Dot(v1, v2);
    EXPECT_FLOAT_EQ(v1.x*v2.x+v1.y*v2.y, result);
}

TEST(Vec2f, Magnitude)
{
    constexpr auto v1= planets::Vec2f{-1.5f, -7.0f};
    constexpr auto result = v1.SquareMagnitude();
    constexpr auto result2 = planets::Vec2f::Dot(v1, v1);
    EXPECT_FLOAT_EQ(result, result2);

    const auto magnitude = v1.Magnitude();
    const auto other = std::sqrt(result);
    EXPECT_FLOAT_EQ(magnitude, other);

    const auto normalized = v1.Normalized();
    const auto normalizedCheck = v1/magnitude;
    EXPECT_FLOAT_EQ(normalized.x, normalizedCheck.x);
    EXPECT_FLOAT_EQ(normalized.y, normalizedCheck.y);
}

TEST(FourFloat, Sqrt)
{
    constexpr std::array<float, 4> numbers = {1.0f, 3.4f, 5.1f, 9.1f};
    constexpr auto four_f = planets::FourFloat (numbers.data());
    const auto result = four_f.Sqrt();
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(result[i], std::sqrt(numbers[i]));
    }
}

TEST(FourVec2f, Constructor)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
            {1.0f, 3.5f},
            {1.0f, -3.5f},
            {-1.0f, 3.5f},
            {-1.0f, -3.5f},
            }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());

    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].x, four_vs.Xs()[i]);
        EXPECT_FLOAT_EQ(vs[i].y, four_vs.Ys()[i]);
    }
}

TEST(FourVec2f, Add)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());
    constexpr std::array<planets::Vec2f, 4> vs2{{
                                                       {1.2f, 3.5f},
                                                       {-2.1f, 3.5f},
                                                       {4.3f, 3.5f},
                                                       {8.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs2 = planets::FourVec2f(vs2.data());

    const auto result = four_vs+four_vs2;
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].x+vs2[i].x, result.Xs()[i]);
        EXPECT_FLOAT_EQ(vs[i].y+vs2[i].y, result.Ys()[i]);
    }
}

TEST(FourVec2f, Sub)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());
    constexpr std::array<planets::Vec2f, 4> vs2{{
                                                        {1.2f, 3.5f},
                                                        {-2.1f, 3.5f},
                                                        {4.3f, 3.5f},
                                                        {8.0f, -3.5f},
                                                }
    };
    constexpr auto four_vs2 = planets::FourVec2f(vs2.data());

    const auto result = four_vs-four_vs2;
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].x-vs2[i].x, result.Xs()[i]);
        EXPECT_FLOAT_EQ(vs[i].y-vs2[i].y, result.Ys()[i]);
    }
}


TEST(FourVec2f, MulFloat)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());
    constexpr std::array<float, 4> ns{{
                                                       1.2f,
                                                       -2.1f,
                                                       4.3f,
                                                        8.0f,
                                                }
    };
    constexpr auto four_ns = planets::FourFloat (ns.data());

    const auto result = four_vs*four_ns;
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].x*ns[i], result.Xs()[i]);
        EXPECT_FLOAT_EQ(vs[i].y*ns[i], result.Ys()[i]);
    }
}


TEST(FourVec2f, DivFloat)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());
    constexpr std::array<float, 4> ns{{
                                              1.2f,
                                              -2.1f,
                                              4.3f,
                                              8.0f,
                                      }
    };
    constexpr auto four_ns = planets::FourFloat(ns.data());

    const auto result = four_vs/four_ns;
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].x/ns[i], result.Xs()[i]);
        EXPECT_FLOAT_EQ(vs[i].y/ns[i], result.Ys()[i]);
    }
}

TEST(FourVec2f, Dot)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());
    constexpr std::array<planets::Vec2f, 4> vs2{{
                                                        {1.2f, 3.5f},
                                                        {-2.1f, 3.5f},
                                                        {4.3f, 3.5f},
                                                        {8.0f, -3.5f},
                                                }
    };
    constexpr auto four_vs2 = planets::FourVec2f(vs2.data());

    const auto result = planets::FourVec2f::Dot(four_vs, four_vs2);
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(planets::Vec2f::Dot(vs[i],vs2[i]), result[i]);
    }
}


TEST(FourVec2f, SquareMagnitude)
{
    constexpr std::array<planets::Vec2f, 4> vs{{
                                                       {1.0f, 3.5f},
                                                       {1.0f, -3.5f},
                                                       {-1.0f, 3.5f},
                                                       {-1.0f, -3.5f},
                                               }
    };
    constexpr auto four_vs = planets::FourVec2f(vs.data());

    const auto result = four_vs.SquareMagnitude();
    for(int i = 0; i < 4; i++)
    {
        EXPECT_FLOAT_EQ(vs[i].SquareMagnitude(), result[i]);
    }
}