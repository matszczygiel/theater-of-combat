cmake_minimum_required(VERSION 3.8)

project(theater-of-combat-new VERSION 0.1.0 LANGUAGES CXX)

add_executable(toc-new
                main.cpp
                source/component.h
                source/component.cpp
                )

target_include_directories(toc-new PUBLIC 
                            source
                            )

target_compile_features(toc-new PUBLIC 
                        cxx_std_17)

target_compile_options(toc-new PUBLIC 
                        -Wall)

target_link_libraries(toc-new PUBLIC )
                