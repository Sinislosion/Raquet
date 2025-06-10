cmake_minimum_required(VERSION 3.1)

project(Raquet)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp" "include/*.h" "include/*.hpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_STANDARD 99)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/Raquet)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE m SDL2 SDL2main SDL2_mixer)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)
