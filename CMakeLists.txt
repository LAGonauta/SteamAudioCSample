cmake_minimum_required(VERSION 3.10)

set(OPENAL_LIBRARY "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\libs\\Win64\\OpenAL32.lib")
set(OPENAL_INCLUDE_DIR "C:\\Program Files (x86)\\OpenAL 1.1 SDK\\include")

project(steamaudio_example)

add_executable(steamaudio_example main.cpp)
set_property(TARGET steamaudio_example PROPERTY CXX_STANDARD 17)

add_subdirectory(Externals/SteamAudio)

add_dependencies(steamaudio_example SteamAudio)
target_link_libraries(steamaudio_example SteamAudio)

option(ALURE_BUILD_EXAMPLES OFF)
option(ALURE_BUILD_STATIC OFF)

add_subdirectory(Externals/alure)
add_dependencies(steamaudio_example alure2)
target_link_libraries(steamaudio_example alure2)

option(BUILD_TESTS OFF)
option(WITH_LSR_BINDINGS OFF)

add_subdirectory(Externals/soxr-code)
add_dependencies(steamaudio_example soxr)
target_link_libraries(steamaudio_example soxr)

if(MSVC)
  target_compile_options(steamaudio_example PRIVATE /W4)
else()
  target_compile_options(steamaudio_example PRIVATE -Wall -Wextra -pedantic)
endif()