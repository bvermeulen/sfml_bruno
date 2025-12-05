cmake_minimum_required(VERSION 3.12)

project(move_weight)
enable_language(C CXX)

set(sources
    move_weight.cpp
)

set(CMAKE_PREFIX_PATH "/home/bvermeulen/Development/cpp/SFML-3.0.2")
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

include_directories(
    includes
    ../include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
add_executable(move_weight ${sources} ${includes})

target_link_libraries(move_weight SFML::Graphics SFML::Window SFML::System)

set_target_properties(move_weight PROPERTIES LDFLAGS "-Wall")
set_target_properties(move_weight PROPERTIES COMPILE_FLAGS "-Wall -g -fvisibility=hidden")
