cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=cppcoreguidelines-*, portability-*")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS 
    source/map/*.cpp            source/map/*.h
    source/gui/*.cpp            source/gui/*.h
    source/graphics/*.cpp       source/graphics/*.h
    source/application/*.cpp    source/application/*.h
    source/core/*.cpp           source/core/*.h
    source/lua/*.cpp            source/lua/*.h
    source/unit/*.cpp           source/unit/*.h
    source/moving/*.cpp         source/moving/*.h
    source/gameplay/*.cpp       source/gameplay/*.h
    source/main.cpp
    )

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(sol2 REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog REQUIRED)
#find_package(cereal REQUIRED)


add_executable(toc ${sources})

target_include_directories(toc PRIVATE 
                            source
                            deps
                            ${LUA_INCLUDE_DIR})

target_compile_features(toc PRIVATE 
                        cxx_std_17)

target_compile_options(toc PRIVATE 
                        -Wall -Wextra -pedantic)

target_compile_definitions(toc PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_PRINT_ERRORS=0)                        


target_link_libraries(toc PRIVATE 
                        sfml-window
                        sfml-graphics
                        sfml-system
                        sfml-network
                        ImGui-SFML::ImGui-SFML
                        ${LUA_LIBRARIES}
                        ) 
                        
enable_testing()           
add_subdirectory(tests)                        

                                            