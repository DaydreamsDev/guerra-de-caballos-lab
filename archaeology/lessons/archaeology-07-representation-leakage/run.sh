#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/parallel_arrays.cpp" -o "${buildDirectory}/parallel-arrays"
"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/record.cpp" -o "${buildDirectory}/record"

parallelOutput="$("${buildDirectory}/parallel-arrays")"
recordOutput="$("${buildDirectory}/record")"

printf 'parallel=%s\n' "${parallelOutput}"
printf 'record=%s\n' "${recordOutput}"
if [[ "${parallelOutput}" != "${recordOutput}" ]]; then
    printf 'match=no\n'
    exit 1
fi
printf 'match=yes\n'
