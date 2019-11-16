add_executable(kirch-moving-test source/kircholm/moving.cpp)
target_include_directories(kirch-moving-test PRIVATE)
target_compile_features(kirch-moving-test PRIVATE cxx_std_17)
target_link_libraries(kirch-moving-test PRIVATE toc-engine toc-systems doctest::doctest)

add_test(moving kirch-moving-test)