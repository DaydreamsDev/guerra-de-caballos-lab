#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
compiler="${CXX:-c++}"
buildDirectory="${scriptDirectory}/build"
binaryPath="${buildDirectory}/seed-control"
mkdir -p "${buildDirectory}"

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/code/seed_control.cpp" -o "${binaryPath}"

firstRun="$("${binaryPath}" --seed 1)"
secondRun="$("${binaryPath}" --seed 1)"
goldenRun="$(cat "${scriptDirectory}/code/tests/golden-seed-1.txt")"

printf 'first=%s\n' "${firstRun}"
if [[ "${firstRun}" != "${secondRun}" ]]; then
    printf 'same=no\n'
    exit 1
fi
printf 'same=yes\n'
if [[ "${firstRun}" != "${goldenRun}" ]]; then
    printf 'golden=no\n'
    exit 1
fi
printf 'golden=yes\n'
