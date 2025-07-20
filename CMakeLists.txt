cmake_minimum_required(VERSION 3.10)
project(crypto-cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_executable(crypto-cpp src/main.cpp src/app_config.cpp src/utils/encrypt.cpp src/utils/getInputBytes.cpp)

add_subdirectory(external/CLI11)
target_link_libraries(crypto-cpp PRIVATE CLI11::CLI11)

# Crypto++ CMake Wrapper
add_subdirectory(external/cryptopp-cmake)

set(CRYPTOPP_SOURCES "${CMAKE_SOURCE_DIR}/external/cryptopp")

set_property(DIRECTORY external/cryptopp-cmake PROPERTY
    CRYPTOPP_SOURCES ${CRYPTOPP_SOURCES})

target_link_libraries(crypto-cpp PRIVATE cryptopp)