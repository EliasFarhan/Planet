cmake_minimum_required (VERSION 3.18)
project(Planets)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(ENABLE_PROFILER "Enable tracy profiling" OFF)
option(ENABLE_ASM_LISTING "Generate assembly listings (MSVC) in <build>/asm/<target>/" ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /w14640 /GL /Oy- /arch:AVX2 /Ob3)
    add_compile_definitions(_USE_MATH_DEFINES)
    add_link_options(/LTCG)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -flto
            -fno-omit-frame-pointer -march=haswell -ffast-math)
    add_link_options(-flto)
endif()

# Assembly listings with interleaved source, one folder per target so files
# compiled by several targets (planet.cpp, vec.cpp) don't overwrite each other.
# With /GL they are written at link time and show the final LTCG code.
function(enable_asm_listing target)
    if (MSVC AND ENABLE_ASM_LISTING)
        set(asm_dir "${CMAKE_CURRENT_BINARY_DIR}/asm/${target}")
        file(MAKE_DIRECTORY "${asm_dir}")
        target_compile_options(${target} PRIVATE /FAs "/Fa${asm_dir}/")
    endif()
endfunction()

find_package(SFML 3 COMPONENTS Graphics CONFIG REQUIRED)

file(GLOB_RECURSE src_files src/*.cpp include/*.h)
add_executable(planets ${src_files})
target_link_libraries(planets PRIVATE SFML::Graphics)
target_include_directories(planets PRIVATE include/)
enable_asm_listing(planets)

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
enable_asm_listing(bench_planet)