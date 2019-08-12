cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

add_executable(toc 
                source/main.cpp
                source/log.cpp
                source/log.h
                source/map/shapes/hex_shape.cpp
                source/map/shapes/hex_shape.h
                source/map/shapes/hex_border_shape.h
                source/map/shapes/hex_border_shape.cpp
                source/map/hexagons.h
                source/map/hexagons.cpp
                source/map/graph.h
                source/map/graph.cpp
                source/map/map_site.h
                source/map/map_site.cpp
                source/map/hex_site.h 
                source/map/hex_site.cpp
                source/map/passage_site.h
                source/map/concrete_passage.h
                source/map/concrete_passage.cpp
                source/map/map.h
                source/map/map.cpp
                source/unit/heavy_unit.cpp
                source/unit/heavy_unit.h
                source/unit/unit.h
                source/unit/unit.cpp
                source/unit/tokenizable.h
                source/unit/tokenizable.cpp
                source/unit/stack.h
                source/unit/stack.cpp
                source/unit/unit_set.h
                source/unit/unit_set.cpp
                source/mover/mover.h
                source/mover/mover.cpp
                source/mover/concrete_mover.h
                source/application/application.h
                source/application/application.cpp
                source/application/game_app.h
                source/application/game_app.cpp
                source/application/player.h
                source/application/player.cpp
                source/battlefield/battlefield.h
                source/battlefield/battlefield.cpp
                source/networking/server.h
                source/networking/server.cpp
                source/networking/client.h
                source/networking/client.cpp
                source/networking/netwoking_status.h
                source/randomizer.h
                source/randomizer.cpp
                source/messaging/messaging.cpp
                source/messaging/messaging.h
                source/messaging/concrete_message.h
                source/messaging/concrete_message.cpp
                source/core/registrable.h
                source/team/team.h
                source/team/team.cpp
                source/gui/imgui_demo.cpp
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

                                            