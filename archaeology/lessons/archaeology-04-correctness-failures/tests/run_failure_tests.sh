#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
archivePath="${1:?usage: run_failure_tests.sh ARCHIVE_PATH}"
compiler="${CXX:-c++}"

scratchDirectory="$(mktemp -d)"
trap 'rm -rf "${scratchDirectory}"' EXIT

"${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 \
    "${scriptDirectory}/failure_tests.cpp" -o "${scratchDirectory}/failure-tests"
"${scratchDirectory}/failure-tests"

baselineVariant="${archivePath}/original/variant-01-baseline-2008"
pointersVariant="${archivePath}/original/variant-03-pointers-dev-2008"
baselineSource="${baselineVariant}/source/guerra-de-caballos-baseline.cpp"
pointersSource="${pointersVariant}/source/guerra-de-caballos-pointers-dev.cpp"

if grep -Eq 'void[[:space:]]+main' "${baselineSource}" &&
    grep -Eq 'int[[:space:]]+main' "${pointersSource}"; then
    printf 'failure-002=detected\n'
else
    printf 'failure-002=not-detected\n'
    exit 1
fi

if grep -q 'cin>>' \
    "${archivePath}/original/variant-02-records-2008/source/guerra-de-caballos-records.cpp"; then
    printf 'failure-006=detected\n'
else
    printf 'failure-006=not-detected\n'
    exit 1
fi
