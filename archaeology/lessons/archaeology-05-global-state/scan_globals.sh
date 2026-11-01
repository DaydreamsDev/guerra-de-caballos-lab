#!/usr/bin/env bash
set -euo pipefail

archivePath="${1:?usage: scan_globals.sh ARCHIVE_PATH}"
sourceFile="${archivePath}/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
test -f "${sourceFile}"

printf 'global_declarations:\n'
grep -nE '^(int|char|const char)[[:space:]]+' "${sourceFile}" | head -n 20

printf 'function_headers:\n'
grep -nE '^[a-zA-Z_][a-zA-Z0-9_ ]*\([^;]*\)[[:space:]]*$' "${sourceFile}" | head -n 30

printf 'global_mentions:\n'
globalNames=(
    vectab vecjugadas cab1 cab2 jugada orden ganador
    cf1 cc1 cf2 cc2 cab simcab1 simcab2 sep
)
for globalName in "${globalNames[@]}"; do
    mentionCount="$(grep -oE "\\b${globalName}\\b" "${sourceFile}" | wc -l)"
    printf '%s=%s\n' "${globalName}" "${mentionCount}"
done
