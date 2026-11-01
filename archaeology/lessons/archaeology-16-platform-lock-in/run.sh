#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
binaryPath="${buildDirectory}/platform-adapter"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/platform.cpp" "${scriptDirectory}/code/main.cpp" \
    -o "${binaryPath}"

output="$("${binaryPath}")"
if [[ "${output}" != *"portable=yes"* ]]; then
    printf 'adapter=no\n'
    exit 1
fi
printf 'adapter=portable\n'
