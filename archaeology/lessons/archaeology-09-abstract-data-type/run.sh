#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/main.cpp" "${scriptDirectory}/code/board_array.cpp" \
    -o "${buildDirectory}/array-representation"
"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/main.cpp" "${scriptDirectory}/code/board_bitmask.cpp" \
    -o "${buildDirectory}/bitmask-representation"

arrayOutput="$("${buildDirectory}/array-representation")"
bitmaskOutput="$("${buildDirectory}/bitmask-representation")"

printf 'array=%s\n' "${arrayOutput}"
printf 'bitmask=%s\n' "${bitmaskOutput}"
if [[ "${arrayOutput}" != "${bitmaskOutput}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'match=yes\n'
