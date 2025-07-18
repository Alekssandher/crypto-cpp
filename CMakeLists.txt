cmake_minimum_required(VERSION 3.10)
project(crypto-cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_executable(crypto-cpp src/main.cpp src/app_config.cpp)

add_subdirectory(external/CLI11)
target_link_libraries(crypto-cpp PRIVATE CLI11::CLI11)
