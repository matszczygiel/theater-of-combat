add_executable(map-test source/map.cpp)
target_compile_features(map-test PRIVATE cxx_std_17)
target_link_libraries(map-test PRIVATE toc-engine doctest::doctest)
add_test(map map-test)

add_executable(units-test source/units.cpp)
target_compile_features(units-test PRIVATE cxx_std_17)
target_link_libraries(units-test PRIVATE toc-engine doctest::doctest)
add_test(units units-test)


add_executable(core-test source/core.cpp)
target_compile_features(core-test PRIVATE cxx_std_17)
target_link_libraries(core-test PRIVATE toc-engine doctest::doctest)
add_test(core core-test)
               

