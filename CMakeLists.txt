cmake_minimum_required(VERSION 3.8)

project(theater-of-combat 
        VERSION 0.3.0 
        LANGUAGES CXX)

        
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
    #set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=cppcoreguidelines-*, portability-*")
    
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)
    
endif()
    
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(tests)                        
                        
enable_testing()           
                                            