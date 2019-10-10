file(GLOB_RECURSE toc_engine_sources CONFIGURE_DEPENDS
    source/map/*.cpp            source/map/*.h
    source/gui/*.cpp            source/gui/*.h
    source/graphics/*.cpp       source/graphics/*.h
    source/core/*.cpp           source/core/*.h
    )

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(sol2 REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog REQUIRED)
#find_package(cereal REQUIRED)

add_library(toc-engine STATIC ${toc_engine_sources})

target_include_directories(toc-engine 
                            PUBLIC include ${LUA_INCLUDE_DIR}
                            PRIVATE include/toc)

target_compile_features(toc-engine PRIVATE 
                        cxx_std_17)

target_compile_options(toc-engine PRIVATE 
                        -Wall -Wextra -pedantic)

target_compile_definitions(toc-engine 
                            PUBLIC SOL_ALL_SAFETIES_ON=1 SOL_PRINT_ERRORS=0) 


target_link_libraries(toc-engine 
                        PUBLIC sfml-window sfml-graphics sfml-system sfml-network
                                ImGui-SFML::ImGui-SFML ${LUA_LIBRARIES}) 
                                            