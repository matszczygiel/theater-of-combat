file(GLOB_RECURSE game_sources CONFIGURE_DEPENDS 
    source/map/*.cpp            source/map/*.h
    source/gui/*.cpp            source/gui/*.h
    source/graphics/*.cpp       source/graphics/*.h
    source/application/*.cpp    source/application/*.h
    source/core/*.cpp           source/core/*.h
    source/lua/*.cpp            source/lua/*.h
    source/unit/*.cpp           source/unit/*.h
    source/systems/*.cpp        source/systems/*.h
    source/gameplay/*.cpp       source/gameplay/*.h
    source/networking/*.cpp     source/networking/*.h
    source/main.cpp
    )

add_executable(toc-game ${game_sources})

target_include_directories(toc-game 
                            PRIVATE source ../deps engine/source)

target_compile_features(toc-game 
                        PRIVATE cxx_std_17)

target_compile_options(toc-game 
                        PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(toc-game 
                        PRIVATE toc-engine) 