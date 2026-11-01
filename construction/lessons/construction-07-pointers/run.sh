#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
sourcePath="${scriptDirectory}/code/pointers-baseline.cpp"
cmakeFilePath="${scriptDirectory}/CMakeLists.txt"
buildDirectory="${scriptDirectory}/build"
binaryPath="${buildDirectory}/pointers-baseline"

if [[ ! -x "${binaryPath}" || "${sourcePath}" -nt "${binaryPath}" ||
      "${cmakeFilePath}" -nt "${binaryPath}" ]]; then
    mkdir -p "${buildDirectory}"
    if command -v cmake >/dev/null 2>&1; then
        cmake -S "${scriptDirectory}" -B "${buildDirectory}" -DCMAKE_BUILD_TYPE=Release
        cmake --build "${buildDirectory}" --parallel
    else
        compiler="${CXX:-c++}"
        "${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
            -o "${binaryPath}" "${sourcePath}"
    fi
fi

exec "${binaryPath}" "$@"
