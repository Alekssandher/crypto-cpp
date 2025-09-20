#!/bin/bash

# Check if dependencies are installed
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "Error: MinGW-w64 is not installed"
    echo "Install it with: sudo apt install mingw-w64"
    exit 1
fi

# Check if libsodium is available
if [ ! -f "/usr/x86_64-w64-mingw32/include/sodium.h" ] && [ ! -f "/usr/local/x86_64-w64-mingw32/include/sodium.h" ]; then
    echo "Warning: libsodium not found for MinGW-w64"
    echo "You may need to install it manually or use a static version"
fi

mkdir -p build-win
cd build-win

# Set environment variables for pkg-config
export PKG_CONFIG_PATH="/usr/x86_64-w64-mingw32/lib/pkgconfig"
export PKG_CONFIG_LIBDIR="/usr/x86_64-w64-mingw32/lib/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="/usr/x86_64-w64-mingw32"

cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-w64.toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_FIND_ROOT_PATH="/usr/x86_64-w64-mingw32" \
      ..

make -j$(nproc)
