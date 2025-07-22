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
if(WIN32 AND CMAKE_CROSSCOMPILING)
    # cross-compilation on Windows
    find_path(SODIUM_INCLUDE_DIR sodium.h
        PATHS /usr/x86_64-w64-mingw32/include
              /usr/local/x86_64-w64-mingw32/include
    )
    
    find_library(SODIUM_LIBRARY
        NAMES sodium libsodium
        PATHS /usr/x86_64-w64-mingw32/lib
              /usr/local/x86_64-w64-mingw32/lib
    )
    
    if(SODIUM_INCLUDE_DIR AND SODIUM_LIBRARY)
        target_include_directories(crypto-cpp PRIVATE ${SODIUM_INCLUDE_DIR})
        target_link_libraries(crypto-cpp PRIVATE ${SODIUM_LIBRARY})
        
        
        target_link_libraries(crypto-cpp PRIVATE ws2_32 advapi32)
    else()
        message(FATAL_ERROR "libsodium not found for cross-compilation")
    endif()
else()
    # linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)
    
    target_include_directories(crypto-cpp PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(crypto-cpp PRIVATE ${SODIUM_LIBRARIES})
endif()