#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
archivePath="${1:?usage: capture_trace.sh ARCHIVE_PATH SEED OUTPUT_PATH}"
seed="${2:?usage: capture_trace.sh ARCHIVE_PATH SEED OUTPUT_PATH}"
outputPath="${3:?usage: capture_trace.sh ARCHIVE_PATH SEED OUTPUT_PATH}"

if [[ ! "${seed}" =~ ^[0-9]+$ ]]; then
    printf 'seed must be a non-negative integer\n' >&2
    exit 2
fi

sourceFile="${archivePath}/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
test -f "${sourceFile}"

scratchDirectory="$(mktemp -d)"
trap 'rm -rf "${scratchDirectory}"' EXIT

cp "${sourceFile}" "${scratchDirectory}/legacy.cpp"
sed -i "s/srand(time(NULL));/srand(${seed});/" "${scratchDirectory}/legacy.cpp"

compiler="${CXX:-c++}"
"${compiler}" -std=c++20 -fpermissive -Dmain=legacy_main \
    -I"${scriptDirectory}/compat" -c "${scratchDirectory}/legacy.cpp" \
    -o "${scratchDirectory}/legacy.o"
"${compiler}" -std=c++20 "${scriptDirectory}/legacy_wrapper.cpp" \
    "${scratchDirectory}/legacy.o" -o "${scratchDirectory}/legacy-probe"

mkdir -p "$(dirname -- "${outputPath}")"
set +e
timeout 5s "${scratchDirectory}/legacy-probe" < /dev/null \
    > "${outputPath}" 2> "${scratchDirectory}/stderr.txt"
runStatus=$?
set -e

stdoutLines="$(wc -l < "${outputPath}")"
stderrLines="$(wc -l < "${scratchDirectory}/stderr.txt")"

printf 'seed=%s\n' "${seed}"
printf 'output=%s\n' "${outputPath}"
printf 'run_status=%s\n' "${runStatus}"
printf 'stdout_lines=%s\n' "${stdoutLines}"
printf 'stderr_lines=%s\n' "${stderrLines}"
