cmake_minimum_required(VERSION 3.14)
project(GobbletGobblers VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the path to SFML - Calea actualizată către SFML
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/temp/SFML-2.6.1-windows-vc17-64-bit")

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Find SFML packages
find_package(SFML 2 REQUIRED graphics window system)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link with SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

# Copy SFML DLL-uri în directorul de build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_PREFIX_PATH}/bin" $<TARGET_FILE_DIR:${PROJECT_NAME}>)