cmake_minimum_required(VERSION 4.0)
project(SDL_Games)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

add_executable(SDL_Games
        main.cpp)

target_link_libraries(SDL_Games PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
set(CMAKE_BUILD_RPATH "/opt/homebrew/lib")