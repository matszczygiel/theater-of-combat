find_package(doctest REQUIRED)

add_executable(map-test source/map.cpp)
target_compile_features(map-test PRIVATE cxx_std_17)
target_link_libraries(map-test PRIVATE toc-engine doctest::doctest)
add_test(map map-test)


file(GLOB_RECURSE moving_test_sources CONFIGURE_DEPENDS 
    ../game/source/moving/*.cpp)

add_executable(moving-test source/moving.cpp ${moving_test_sources})
target_include_directories(moving-test PRIVATE ../game/source ../deps)
target_compile_features(moving-test PRIVATE cxx_std_17)
target_link_libraries(moving-test PRIVATE toc-engine doctest::doctest)
add_test(moving moving-test)



file(GLOB_RECURSE units_test_sources CONFIGURE_DEPENDS 
    ../game/source/unit/*.cpp)

add_executable(units-test source/units.cpp ${units_test_sources})
target_include_directories(units-test PRIVATE ../game/source ../deps)
target_compile_features(units-test PRIVATE cxx_std_17)
target_link_libraries(units-test PRIVATE toc-engine doctest::doctest)
add_test(units units-test)


add_executable(core-test source/core.cpp)
target_compile_features(core-test PRIVATE cxx_std_17)
target_link_libraries(core-test PRIVATE toc-engine doctest::doctest)
add_test(core core-test)
               

