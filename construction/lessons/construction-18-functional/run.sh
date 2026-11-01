#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
sourceDirectory="${scriptDirectory}/code"
cmakeFilePath="${scriptDirectory}/CMakeLists.txt"
buildDirectory="${scriptDirectory}/build"
binaryPath="${buildDirectory}/functional-baseline"

headerNeedsRebuild=false
while IFS= read -r headerPath; do
    if [[ "${headerPath}" -nt "${binaryPath}" ]]; then
        headerNeedsRebuild=true
        break
    fi
done < <(find "${sourceDirectory}" -name '*.h' -print)

if [[ ! -x "${binaryPath}" || "${cmakeFilePath}" -nt "${binaryPath}" ||
      "${headerNeedsRebuild}" == true ||
      "${sourceDirectory}/main.cpp" -nt "${binaryPath}" ||
      "${sourceDirectory}/game.cpp" -nt "${binaryPath}" ||
      "${sourceDirectory}/knight.cpp" -nt "${binaryPath}" ||
      "${sourceDirectory}/position.cpp" -nt "${binaryPath}" ||
      "${sourceDirectory}/random_sequence.cpp" -nt "${binaryPath}" ]]; then
    mkdir -p "${buildDirectory}"
    if command -v cmake >/dev/null 2>&1; then
        cmake -S "${scriptDirectory}" -B "${buildDirectory}" -DCMAKE_BUILD_TYPE=Release 1>&2
        cmake --build "${buildDirectory}" --parallel 1>&2
    else
        compiler="${CXX:-c++}"
        "${compiler}" -std=c++20 -Wall -Wextra -Werror -O2 -I"${sourceDirectory}" \
            -o "${binaryPath}" \
            "${sourceDirectory}/main.cpp" \
            "${sourceDirectory}/game.cpp" \
            "${sourceDirectory}/knight.cpp" \
            "${sourceDirectory}/position.cpp" \
            "${sourceDirectory}/random_sequence.cpp"
    fi
fi

exec "${binaryPath}" "$@"
