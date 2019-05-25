cmake_minimum_required(VERSION 3.8)

project(test VERSION 0.1.0 LANGUAGES CXX)

add_executable(test 
                ../../source/map/shapes/hex_shape.cpp
                ../../source/map/shapes/hex_shape.h
                ../../source/core/registrable.h
                ../../source/map/map_site.h
                ../../source/map/map_site.cpp
                ../../source/map/hex_site.h
                ../../source/map/hex_site.cpp
                ../../source/map/concrete_hex.h
                main.cpp
                )

target_include_directories(test PUBLIC 
                            ../../source
                            )

target_compile_features(test PUBLIC 
                        cxx_std_17)

target_compile_options(test PUBLIC 
                        -Wall -Wextra -pedantic)

target_link_libraries(test PUBLIC 
                        sfml-window
                        sfml-graphics
                        sfml-system
                        sfml-network
                        pugixml)   