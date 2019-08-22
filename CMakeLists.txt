cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=cppcoreguidelines-*, portability-*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS 
    source/map/*.cpp            source/map/*.h
    source/gui/*.cpp            source/gui/*.h
    source/graphics/*.cpp       source/graphics/*.h
    source/application/*.cpp    source/application/*.h
    source/main.cpp
    source/log.h source/log.cpp 
    )

add_executable(toc ${sources})

target_include_directories(toc PRIVATE 
                            source)

target_compile_features(toc PRIVATE 
                        cxx_std_17)

target_compile_options(toc PRIVATE 
                        -Wall -Wextra -pedantic)

find_package(ImGui-SFML REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(toc PRIVATE 
                        sfml-window
                        sfml-graphics
                        sfml-system
                        sfml-network
                        ImGui-SFML::ImGui-SFML) 
                        
enable_testing()           
add_subdirectory(tests)                        

                                            