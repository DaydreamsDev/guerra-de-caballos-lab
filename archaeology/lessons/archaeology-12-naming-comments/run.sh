#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/before.cpp" -o "${buildDirectory}/before"
"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/after.cpp" -o "${buildDirectory}/after"

beforeOutput="$("${buildDirectory}/before")"
afterOutput="$("${buildDirectory}/after")"

printf 'before=%s\n' "${beforeOutput}"
printf 'after=%s\n' "${afterOutput}"
if [[ "${beforeOutput}" != "${afterOutput}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'match=yes\n'
