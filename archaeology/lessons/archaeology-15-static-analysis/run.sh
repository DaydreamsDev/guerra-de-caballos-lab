#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -O2 \
    "${scriptDirectory}/code/warnings.cpp" -o "${buildDirectory}/warnings" \
    2> "${buildDirectory}/warnings.txt"

warningCount="$(grep -c 'warning:' "${buildDirectory}/warnings.txt" || true)"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/fixed.cpp" -o "${buildDirectory}/fixed"

warningOutput="$("${buildDirectory}/warnings")"
fixedOutput="$("${buildDirectory}/fixed")"

printf 'warnings=%s\n' "${warningCount}"
if [[ "${warningOutput}" != "${fixedOutput}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'fixed=ok\n'
printf 'match=yes\n'
