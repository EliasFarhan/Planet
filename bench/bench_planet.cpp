#include "planet.h"
#include <benchmark/benchmark.h>

constexpr long fromRange = 8;

constexpr long toRange = 1 << 15;

static void BM_Update1(benchmark::State& state)
{
    planets::PlanetSystem planetSystem(state.range(0));
    for (auto _ : state)
    {
        planetSystem.Update(0.166f);
    }
}
BENCHMARK(BM_Update1)->Range(fromRange, toRange);

static void BM_Update4(benchmark::State& state)
{
    planets::PlanetSystem4 planetSystem(state.range(0));
    for (auto _ : state)
    {
        planetSystem.Update(0.166f);
    }
}
BENCHMARK(BM_Update4)->Range(fromRange, toRange);

static void BM_Update8(benchmark::State& state)
{
    planets::PlanetSystem8 planetSystem(state.range(0));
    for (auto _ : state)
    {
        planetSystem.Update(0.166f);
    }
}
BENCHMARK(BM_Update8)->Range(fromRange, toRange);