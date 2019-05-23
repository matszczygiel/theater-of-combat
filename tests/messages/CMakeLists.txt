cmake_minimum_required(VERSION 3.8)

project(test VERSION 0.1.0 LANGUAGES CXX)

add_executable(test 
                ../../source/messaging/messaging.cpp
                ../../source/messaging/messaging.h
                ../../source/messaging/concrete_message.h
                ../../source/messaging/concrete_message.cpp
                ../../source/core/registrable.h
                main.cpp
                )

target_include_directories(test PUBLIC 
                            ../../source
                            )

target_compile_features(test PUBLIC 
                        cxx_std_17)

target_compile_options(test PUBLIC 
                        -Wall -Wextra -pedantic)