cmake_minimum_required(VERSION 3.14)
project(Skeleton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG b75ecf1bed2fcd416b66c86cb6fe79122abf132e
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(SOURCE
    examples/main_example.cc
    examples/spawns.h
    Skeleton/eve/ewar_modules.h
    Skeleton/eve/ewar_modules.cc
    Skeleton/eve/eve_math.h
    Skeleton/eve/eve_math.cc
    Skeleton/eve/effects.h
    Skeleton/eve/effects.cc
    Skeleton/eve/player_ship.h
    Skeleton/eve/ship.h
    Skeleton/eve/ship.cc
    Skeleton/eve/npc.h
    Skeleton/eve/npc.cc
    Skeleton/eve/npc_container.h
    Skeleton/eve/npc_container.cc
    Skeleton/eve/ship_weapon.h
    Skeleton/eve/ship_weapon.cc
    Skeleton/eve/hp_resistances.h
    Skeleton/eve/hp_resistances.cc
    Skeleton/eve/type_converter.h
    Skeleton/eve/type_converter.cc
    Skeleton/db/db_loader.h
    Skeleton/db/db_loader.cc
    Skeleton/abyss_bot/npc_ship.h
    Skeleton/abyss_bot/npc_ship.cc
    Skeleton/abyss_bot/npc_builder.h
    Skeleton/abyss_bot/npc_builder.cc
    tests/effect_tests.cc
    tests/math_tests.cc
)

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME}
  gtest
  gtest_main
)