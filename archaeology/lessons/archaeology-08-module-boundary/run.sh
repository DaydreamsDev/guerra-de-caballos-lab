#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
binaryPath="${buildDirectory}/module-boundary"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/board.cpp" "${scriptDirectory}/code/main.cpp" \
    -o "${binaryPath}"
exec "${binaryPath}" "$@"
