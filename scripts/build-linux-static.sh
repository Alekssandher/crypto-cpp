#!/bin/bash

mkdir -p build-linux-static
cd build-linux-static
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" -DBUILD_SHARED_LIBS=OFF -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++"

make -j$(nproc)