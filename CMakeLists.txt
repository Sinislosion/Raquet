cmake_minimum_required(VERSION 3.5)

set(GAME_NAME Raquet CACHE STRING "The name of the final executable")

set(CMAKE_C_FLAGS "-O2 -Wall -Wextra -Werror -flto")

project(${GAME_NAME})

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp" "include/*.h" "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} program.rc)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_STANDARD 99)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/Raquet)

if(WIN32)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mwindows)
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC m SDL3 SDL3_mixer)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)
