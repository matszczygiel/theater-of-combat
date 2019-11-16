#file(GLOB_RECURSE kirch_moving_test_sources CONFIGURE_DEPENDS 
#../game/source/moving/*.cpp)
#
#add_executable(kirch-moving-test source/kircholm/moving.cpp ${moving_test_sources})
#target_include_directories(moving-test PRIVATE ../game/source ../deps)
#target_compile_features(moving-test PRIVATE cxx_std_17)
#target_link_libraries(moving-test PRIVATE toc-engine doctest::doctest)
#add_test(moving moving-test)
