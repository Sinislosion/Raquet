cmake_minimum_required(VERSION 3.14)

set(GAME_NAME Raquet CACHE STRING "The name of the final executable")

project(${GAME_NAME})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include(CheckIPOSupported)
include(CheckPIESupported)

message("Checking for LTO support")
check_ipo_supported(RESULT LTO_Supported)

if(LTO_Supported)
  set(PROPERTY INTERPROCEDURAL_OPTIMIZATION ON)
  message("LTO is supported and enabled for this build")
else()
  message("LTO is not supported by the compiler and is disabled for this build")
endif()

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG f87239e71e42da91ca317a12eefb82cfbf3393eb
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
    GIT_TAG 72a81869b45e249e8e67102db4e98dd2441f05a1
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(SDL3_mixer)

set(SOURCE_FILES
  src/main.c

  src/Raquet/Raquet_Main.c
  src/Raquet/Raquet_Actor.c
  src/Raquet/Raquet_CHR.c
  src/Raquet/Raquet_Input.c
  src/Raquet/Raquet_Math.c
  src/Raquet/Raquet_Misc.c
  src/Raquet/Raquet_Palette.c
  src/Raquet/Raquet_PPF.c
  src/Raquet/Raquet_Sound.c
  src/Raquet/Raquet_Video.c
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} program.rc)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/Raquet include/)

if (MSVC)
  add_compile_options(/W4 /WX /Oyitx)
else()
  add_compile_options(-Wall -Wpedantic -std=c++98 -Werror -Ofast)
  if (WIN32)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -static)
  endif()
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer m)

add_custom_command (
  TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
  "$<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>" USES_TERMINAL COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets
  $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets
)
