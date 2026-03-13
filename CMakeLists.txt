cmake_minimum_required(VERSION 3.16)
project(Pong)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# on Web targets, we need CMake to generate a HTML webpage.
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

add_executable(Pong
        main.cpp
        Source/box.cpp
        Source/texture.cpp
        Source/globals.cpp
        Source/util.cpp
        Source/exitState.cpp
        Source/introState.cpp
        Source/pongState.cpp
        Source/scoreState.cpp
        Source/audio.cpp
        Source/menuState.cpp
)

if(EMSCRIPTEN)
  set_target_properties(Pong PROPERTIES OUTPUT_NAME "index")
endif()

target_link_libraries(Pong PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer)

if(EMSCRIPTEN)
  target_link_options(Pong PRIVATE --preload-file ${CMAKE_SOURCE_DIR}/Assets@Assets)
endif()