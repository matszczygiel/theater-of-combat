cmake_minimum_required(VERSION 3.8)

project(theater-of-combat-new VERSION 0.1.0 LANGUAGES CXX)

add_executable(toc-new 
 
                )

target_include_directories(toc PUBLIC 
                            source
                            )

target_compile_features(toc PUBLIC 
                        cxx_std_17)

target_compile_options(toc PUBLIC 
                        -Wall)

target_link_libraries(toc PUBLIC )
)                