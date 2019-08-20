cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

add_executable(toc 
                source/main.cpp
                source/log.cpp
                source/log.h
                source/map/hexagons.h
                source/map/hexagons.cpp
                source/map/types.h
                source/map/types.cpp
                source/map/graph.h
                source/map/graph.cpp
                source/map/map.h
                source/map/map.cpp
                source/application/application.h
                source/application/application.cpp
                source/application/game_app.h
                source/application/game_app.cpp
                source/randomizer.h
                source/randomizer.cpp
                source/gui/imgui_demo.cpp
                source/graphics/shapes.cpp
                source/graphics/shapes.h
                source/graphics/map_gfx.cpp
                source/graphics/map_gfx.h
                )

target_include_directories(toc PRIVATE 
                            source
                            )

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
                        
                        
add_subdirectory(tests)                        

                                            