cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

add_executable(toc 
                source/main.cpp
                source/log.cpp
                source/log.h
                source/map/hex_shape.cpp
                source/map/hex_shape.h
                source/map/map_site.h
                source/map/hex_site.h 
                source/map/hex_site.cpp
                source/map/map.h
                source/map/map.cpp
                source/map/directions.h
                source/map/concrete_hex.h
                source/unit/heavy_unit.h
                source/unit/unit_type.h
                source/unit/unit.h
                source/unit/unit.cpp
                source/mover/mover.h
                source/mover/mover.cpp
                source/application/application.h
                source/application/application.cpp
                source/application/game_app.h
                source/application/game_app.cpp
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