#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
archivePath="${1:?usage: build_probe.sh ARCHIVE_PATH}"
sourceFile="${archivePath}/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"

test -f "${sourceFile}"

scratchDirectory="$(mktemp -d)"
trap 'rm -rf "${scratchDirectory}"' EXIT

cp "${sourceFile}" "${scratchDirectory}/legacy.cpp"
compiler="${CXX:-c++}"

"${compiler}" -std=c++20 -fpermissive -Dmain=legacy_main \
    -I"${scriptDirectory}/compat" -c "${scratchDirectory}/legacy.cpp" \
    -o "${scratchDirectory}/legacy.o"

"${compiler}" -std=c++20 "${scriptDirectory}/legacy_wrapper.cpp" \
    "${scratchDirectory}/legacy.o" -o "${scratchDirectory}/legacy-probe"

set +e
timeout 3s "${scratchDirectory}/legacy-probe" < /dev/null \
    > "${scratchDirectory}/stdout.txt" 2> "${scratchDirectory}/stderr.txt"
runStatus=$?
set -e

stdoutLines="$(wc -l < "${scratchDirectory}/stdout.txt")"
stderrLines="$(wc -l < "${scratchDirectory}/stderr.txt")"
stderrFirst="$(head -n 1 "${scratchDirectory}/stderr.txt")"

printf 'source=%s\n' "${sourceFile}"
printf 'compile=ok\n'
printf 'run_status=%s\n' "${runStatus}"
printf 'stdout_lines=%s\n' "${stdoutLines}"
printf 'stderr_lines=%s\n' "${stderrLines}"
printf 'stderr_first=%s\n' "${stderrFirst}"
