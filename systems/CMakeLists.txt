file(GLOB_RECURSE toc_systems_sources CONFIGURE_DEPENDS
    source/*.cpp                include/tsys/*.h
    source/kircholm/*.cpp       include/tsys/kircholm/*.h
    )

add_library(toc-systems STATIC ${toc_systems_sources})

target_compile_features(toc-systems PRIVATE cxx_std_17)

target_include_directories(toc-systems 
                                PUBLIC include 
                                PRIVATE source include/tsys)

if(UNIX)                        
    target_compile_options(toc-systems PRIVATE 
                                     -Wall -Wextra -pedantic)
else()
    if(MSVC)
        target_compile_options(toc-systems PRIVATE 
                                         /MT /w)
    endif()                                         
endif()

target_link_libraries(toc-systems 
                        PUBLIC toc-engine)
                                
                                            