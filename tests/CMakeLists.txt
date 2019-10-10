find_package(doctest REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(sol2 REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE map_sources CONFIGURE_DEPENDS 
    ../source/map/*.cpp
    ../source/core/log.cpp
    )
add_executable(map-test source/map.cpp ${map_sources})
target_include_directories(map-test PRIVATE ../source)
target_compile_features(map-test PRIVATE cxx_std_17)
target_link_libraries(map-test PRIVATE doctest::doctest)
add_test(map map-test)


file(GLOB_RECURSE moving_sources CONFIGURE_DEPENDS 
    ../source/moving/*.cpp
    ../source/map/*.cpp
    ../source/core/log.cpp
    )
add_executable(moving-test source/moving.cpp ${moving_sources})
target_include_directories(moving-test PRIVATE ../source)
target_compile_features(moving-test PRIVATE cxx_std_17)
target_link_libraries(moving-test PRIVATE doctest::doctest)
add_test(moving moving-test)



file(GLOB_RECURSE units_sources CONFIGURE_DEPENDS 
    ../source/unit/*.cpp
    ../source/map/*.cpp
    ../source/core/log.cpp
    )
add_executable(units-test source/units.cpp ${units_sources})
target_include_directories(units-test PRIVATE ../source ../deps)
target_compile_features(units-test PRIVATE cxx_std_17)
target_link_libraries(units-test PRIVATE doctest::doctest)
add_test(units units-test)


file(GLOB_RECURSE core_sources CONFIGURE_DEPENDS 
    ../source/core/*.cpp
    )
add_executable(core-test source/core.cpp ${core_sources})
target_include_directories(core-test PRIVATE ../source ${LUA_INCLUDE_DIR})
target_compile_features(core-test PRIVATE cxx_std_17)
target_link_libraries(core-test PRIVATE doctest::doctest ${LUA_LIBRARIES})
target_compile_definitions(core-test PRIVATE SOL_ALL_SAFETIES_ON=1 SOL_PRINT_ERRORS=0)                        
add_test(core core-test)
               

