cmake_minimum_required(VERSION 3.8)

project(theater-of-combat VERSION 0.1.0 LANGUAGES CXX)

add_executable(toc 
                source/main.cpp
                source/log.cpp
                source/log.h
                source/map/hex_shape.cpp
                source/map/hex_shape.h
                source/map/hex_border_shape.h
                source/map/hex_border_shape.cpp
                source/map/map_site.h
                source/map/map_site.cpp
                source/map/hex_site.h 
                source/map/hex_site.cpp
                source/map/passage_site.h
                source/map/passage_site.cpp
                source/map/concrete_passage.h
                source/map/concrete_passage.cpp
                source/map/map.h
                source/map/map.cpp
                source/map/directions.h
                source/map/concrete_hex.h
                source/map/site_factory.h
                source/map/site_factory.cpp
                source/unit/heavy_unit.h
                source/unit/unit_type.h
                source/unit/unit.h
                source/unit/unit.cpp
                source/unit/tokenizable.h
                source/unit/tokenizable.cpp
                source/unit/stack.h
                source/unit/stack.cpp
                source/mover/mover.h
                source/mover/mover.cpp
                source/mover/concrete_mover.h
                source/application/application.h
                source/application/application.cpp
                source/application/game_app.h
                source/application/game_app.cpp
                source/application/player.h
                source/application/player.cpp
                source/battlefield/battlefield.h
                source/battlefield/battlefield.cpp
                source/networking/server.h
                source/networking/server.cpp
                source/networking/client.h
                source/networking/client.cpp
                source/networking/netwoking_status.h
                source/randomizer.h
                source/randomizer.cpp
                source/messaging/messaging.cpp
                source/messaging/messaging.h
                source/messaging/concrete_message.h
                source/core/registrable.h
                )

target_include_directories(toc PUBLIC 
                            source
                            )

target_compile_features(toc PUBLIC 
                        cxx_std_17)

target_compile_options(toc PUBLIC 
                        -Wall)

target_link_libraries(toc PUBLIC 
                        sfml-window
                        sfml-graphics
                        sfml-system
                        sfml-network
                        pugixml
                        tgui)                