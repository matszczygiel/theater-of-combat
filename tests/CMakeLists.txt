cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

find_package(doctest REQUIRED)

add_executable(map-test source/map.cpp)
target_compile_features(map-test PRIVATE cxx_std_17)
target_link_libraries(map-test PRIVATE doctest::doctest)

add_test(test map-test)
               

