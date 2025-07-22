cmake_minimum_required(VERSION 3.10)
project(crypto-cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_executable(
    crypto-cpp 
    src/main.cpp 
    src/app_config.cpp 
    src/utils/encrypt.cpp 
    src/utils/decrypt.cpp
    src/utils/deleteOriginalFile.cpp
    src/utils/getInputBytes.cpp 
    src/utils/saveFile.cpp 
    src/handlers/handle_args.cpp
)

add_subdirectory(external/CLI11)
target_link_libraries(crypto-cpp PRIVATE CLI11::CLI11)


# Sodium
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

target_include_directories(crypto-cpp PRIVATE ${SODIUM_INCLUDE_DIRS})
target_link_libraries(crypto-cpp PRIVATE ${SODIUM_LIBRARIES})
