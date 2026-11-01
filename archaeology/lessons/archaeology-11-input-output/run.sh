#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/core.cpp" "${scriptDirectory}/code/console.cpp" \
    "${scriptDirectory}/code/main.cpp" -o "${buildDirectory}/console-app"
"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/core.cpp" "${scriptDirectory}/code/test.cpp" \
    -o "${buildDirectory}/core-test"

consoleLines="$("${buildDirectory}/console-app" | wc -l)"
testOutput="$("${buildDirectory}/core-test")"

printf 'console_lines=%s\n' "${consoleLines}"
printf 'test_output=%s\n' "${testOutput}"
if [[ "${testOutput}" != *"test=ok"* ]]; then
    exit 1
fi
printf 'match=yes\n'
