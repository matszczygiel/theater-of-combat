cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

find_package(doctest REQUIRED)

file(GLOB_RECURSE map_sources CONFIGURE_DEPENDS 
    ../source/map/*.cpp
    ../source/core/log.cpp
    )
add_executable(map-test source/map.cpp ${map_sources})
target_include_directories(map-test PRIVATE ../source)
target_compile_features(map-test PRIVATE cxx_std_17)
target_link_libraries(map-test PRIVATE doctest::doctest)
add_test(map map-test)


file(GLOB_RECURSE moving_sources CONFIGURE_DEPENDS 
    ../source/moving/*.cpp
    ../source/map/*.cpp
    ../source/core/log.cpp
    )
add_executable(moving-test source/moving.cpp ${moving_sources})
target_include_directories(moving-test PRIVATE ../source)
target_compile_features(moving-test PRIVATE cxx_std_17)
target_link_libraries(moving-test PRIVATE doctest::doctest)
add_test(moving moving-test)
               

