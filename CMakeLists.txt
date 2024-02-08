cmake_minimum_required(VERSION 3.9)
project(Skeleton LANGUAGES CXX)

set(SOURCE
    examples/main_example.cc
    Skeleton/eve/eve_math.h
    Skeleton/eve/eve_math.cc
    Skeleton/eve/effects.h
    Skeleton/eve/effects.cc
    Skeleton/eve/npc_ship.h
    Skeleton/eve/player_ship.h
    Skeleton/eve/ship.h
    Skeleton/eve/ship.cc
    Skeleton/eve/ship_weapon.h
    Skeleton/eve/ship_weapon.cc
    Skeleton/eve/hp_resistances.h
    Skeleton/eve/hp_resistances.cc
    Skeleton/db/db_loader.h
    Skeleton/db/db_loader.cc
    )

add_executable(${PROJECT_NAME} ${SOURCE})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED on)