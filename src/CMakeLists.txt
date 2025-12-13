cmake_minimum_required(VERSION 3.12)

project(double_pendulum)
enable_language(C CXX)

set(sources
    double_pendulum.cpp
    double_pendulum_sim.cpp
    double_pendulum_view.cpp
)

set(CMAKE_PREFIX_PATH "d:/cpp/SFML-3.0.2")
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

include_directories(
    includes
    ../include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
add_executable(double_pendulum ${sources} ${includes})

target_link_libraries(double_pendulum SFML::Graphics SFML::Window SFML::System)

set_target_properties(double_pendulum PROPERTIES LDFLAGS "-Wall")
set_target_properties(double_pendulum PROPERTIES COMPILE_FLAGS "-Wall -g -fvisibility=hidden")
