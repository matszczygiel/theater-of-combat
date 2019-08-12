cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

enable_testing()
add_executable(tests
                source/map.cpp)
add_test(test tests)

target_compile_features(tests PRIVATE 
                        cxx_std_17)
        
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(tests PRIVATE Threads::Threads gtest gtest_main)

