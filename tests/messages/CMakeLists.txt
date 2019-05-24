cmake_minimum_required(VERSION 3.8)

project(test VERSION 0.1.0 LANGUAGES CXX)

add_executable(test 
                main.cpp
                msg_cereal.h
                msg_cereal.cpp
                )

target_include_directories(test PUBLIC 
                            )

target_compile_features(test PUBLIC 
                        cxx_std_17)

target_compile_options(test PUBLIC 
                        -Wall -Wextra -pedantic)