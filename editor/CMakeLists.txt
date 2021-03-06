file(GLOB_RECURSE editor_sources CONFIGURE_DEPENDS 
    source/application/*.cpp    source/application/*.h
    source/gui/*.cpp            source/gui/*.h
    source/main.cpp
    )

add_executable(toc-editor ${editor_sources})

target_compile_features(toc-editor 
                        PRIVATE cxx_std_17)

target_include_directories(toc-editor 
                            PRIVATE source)

if(UNIX)                        
    target_compile_options(toc-editor PRIVATE 
                                     -Wall -Wextra -pedantic)
else()
    if(MSVC)
        target_compile_options(toc-editor PRIVATE 
                                         /MT /w)
    endif()                                         
endif()

target_link_libraries(toc-editor 
                        PRIVATE toc-engine toc-systems)