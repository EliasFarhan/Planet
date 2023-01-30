#include "planet.h"
#include <benchmark/benchmark.h>

const long fromRange = 8;

const long toRange = 1 << 15;

static void BM_Update1(benchmark::State& state)
{
    planets::PlanetSystem planetSystem(state.range(0));
    // Perform setup here
    for (auto _ : state)
    {
        planetSystem.Update(0.166f);
    }
}
BENCHMARK(BM_Update1)->Range(fromRange, toRange);

static void BM_Update4(benchmark::State& state)
{
    planets::PlanetSystem4 planetSystem(state.range(0));
    // Perform setup here
    for (auto _ : state)
    {
        planetSystem.Update(0.166f);
    }
}
BENCHMARK(BM_Update4)->Range(fromRange, toRange);