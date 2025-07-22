#!/bin/bash

# Verificar se as dependências estão instaladas
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "Erro: MinGW-w64 não está instalado"
    echo "Instale com: sudo apt install mingw-w64"
    exit 1
fi

# Verificar se libsodium está disponível
if [ ! -f "/usr/x86_64-w64-mingw32/include/sodium.h" ] && [ ! -f "/usr/local/x86_64-w64-mingw32/include/sodium.h" ]; then
    echo "Aviso: libsodium não encontrado para MinGW-w64"
    echo "Você pode precisar instalá-lo manualmente ou usar uma versão estática"
fi

mkdir -p build-win
cd build-win

# Definir variáveis de ambiente para pkg-config
export PKG_CONFIG_PATH="/usr/x86_64-w64-mingw32/lib/pkgconfig"
export PKG_CONFIG_LIBDIR="/usr/x86_64-w64-mingw32/lib/pkgconfig"
export PKG_CONFIG_SYSROOT_DIR="/usr/x86_64-w64-mingw32"

cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-w64.toolchain.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_FIND_ROOT_PATH="/usr/x86_64-w64-mingw32" \
      ..

make -j$(nproc)