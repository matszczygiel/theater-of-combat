file(GLOB_RECURSE toc_systems_sources CONFIGURE_DEPENDS
    source/gameplay/*.cpp       source/gameplay/*.h
    )

add_library(toc-systems STATIC ${toc_systems_sources})

target_include_directories(toc-systems PRIVATE source include include/tsys)

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
                                
                                            