cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

add_executable(toc 
                source/main.cpp
                source/log.cpp
                source/log.h
                source/hex_shape.cpp
                source/hex_shape.h
                source/map_site.h
                source/hex_site.h 
                source/hex_site.cpp
                source/unit.h
                source/unit.cpp
                source/map.h
                source/map.cpp
                source/directions.h
                source/mover.h
                source/mover.cpp
                source/application.h
                source/application.cpp
                source/game_app.h
                source/game_app.cpp
                )

target_include_directories(toc PUBLIC 
                            source
                            )

target_compile_features(toc PUBLIC 
                        cxx_std_17)

target_link_libraries(toc PUBLIC 
                        sfml-window
                        sfml-graphics
                        sfml-system)                