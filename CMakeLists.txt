cmake_minimum_required(VERSION 3.9)

set(GAME_NAME Raquet CACHE STRING "The name of the final executable")

project(${GAME_NAME})

find_package(SDL3 CONFIG REQUIRED)

include(FetchContent)

FetchContent_Declare(SDL3_mixer GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer GIT_TAG 30c1301055a35ee87b8679279b6fc88e10d28fa3 )
FetchContent_MakeAvailable(SDL3_mixer)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp" "include/*.h" "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} program.rc)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/Raquet)

if(WIN32)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mwindows)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
  )
