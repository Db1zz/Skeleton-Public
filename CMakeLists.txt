cmake_minimum_required(VERSION 3.6.1)
project(Skeleton LANGUAGES CXX)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for downloading libraries
include(FetchContent)

# Google Test lib
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
# end google test

if (APPLE)
  message(STATUS "Target system MacOS")
  find_Library (APPLICATION_SERVICES_FRAMEWORK ApplicationServices)
endif()

# OpenCV should be installed on your pc
# or this function will throw error.
find_package(OpenCV REQUIRED)

# path to libs
set(Leptonica_DIR "/usr/local/Cellar/leptonica/1.84.1/lib/cmake")
find_package(Leptonica REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET tesseract)
add_library(Tesseract::libtesseract ALIAS PkgConfig::Tesseract)

set(SOURCE
    examples/main_example.cc
    examples/spawns.h
    Skeleton/eve/ewar_modules.h
    Skeleton/eve/ewar_modules.cc
    Skeleton/eve/eve_math.h
    Skeleton/eve/eve_math.cc
    Skeleton/eve/effects.h
    Skeleton/eve/effects.cc
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
    tests/effect_tests.cc
    tests/math_tests.cc
    tests/image_read_test.cc
    Skeleton/third_party/wincap/wincap.cc
    Skeleton/third_party/wincap/wincap.h
    Skeleton/third_party/vision/vision_object.cc
    Skeleton/third_party/vision/vision_object.h
    Skeleton/third_party/img_reader/reader.cc
    Skeleton/third_party/img_reader/reader.h
)

# main executable
add_executable(${PROJECT_NAME} ${SOURCE})

message(STATUS "Leptonica source dir: ${leptonica_SOURCE_DIR}")
message(STATUS "Leptonica binary dir: ${leptonica_BINARY_DIR}")

# apple specific libraries
if (APPLE)
  target_link_libraries(${PROJECT_NAME} ${APPLICATION_SERVICES_FRAMEWORK})
endif()

# include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  ${OpenCV_INCLUDE_DIRS}
  ${Leptonica_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
)

# link libraries
target_link_libraries(${PROJECT_NAME}
  gtest
  ${OpenCV_LIBS}
  ${Leptonica_LIBS}
  Tesseract::libtesseract
)
