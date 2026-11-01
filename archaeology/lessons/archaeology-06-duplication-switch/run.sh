#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
binaryPath="${scriptDirectory}/build/movement"
mkdir -p "${scriptDirectory}/build"
"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/movement.cpp" -o "${binaryPath}"
exec "${binaryPath}" "$@"
