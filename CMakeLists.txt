cmake_minimum_required (VERSION 3.18)
project(Planets)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(ENABLE_PROFILER "Enable tracy profiling" OFF)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /w14640 /GL /Oy- /arch:AVX2 /Fa /Ob3)
    add_compile_definitions(_USE_MATH_DEFINES)
    add_link_options(/LTCG)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -flto
            -fno-omit-frame-pointer -march=haswell -ffast-math)
    add_link_options(-flto)
endif()

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

file(GLOB_RECURSE src_files src/*.cpp include/*.h)
add_executable(planets ${src_files})
target_link_libraries(planets PRIVATE sfml-system sfml-graphics sfml-window)
target_include_directories(planets PRIVATE include/)

if(ENABLE_PROFILER)
    find_package(Tracy CONFIG REQUIRED)
    add_compile_definitions(TRACY_ENABLE)
    target_link_libraries(planets PRIVATE Tracy::TracyClient)
endif (ENABLE_PROFILER)

find_package(GTest CONFIG REQUIRED)
add_executable(test test/test_vec.cpp src/vec.cpp)
target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(test PRIVATE include/)

find_package(benchmark CONFIG REQUIRED)
add_executable(bench_planet bench/bench_planet.cpp src/planet.cpp src/vec.cpp)
target_include_directories(bench_planet PRIVATE include/)
target_link_libraries(bench_planet PRIVATE benchmark::benchmark benchmark::benchmark_main)