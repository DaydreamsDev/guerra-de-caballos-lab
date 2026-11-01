#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
labRoot="$(cd -- "${scriptDirectory}/../../.." && pwd)"
lessonDirectory="${1:?usage: measure.sh LESSON_DIRECTORY}"
sourceDirectory="${lessonDirectory}/code"

fileCount="$(
    find "${sourceDirectory}" -maxdepth 1 -type f \
        \( -name '*.h' -o -name '*.cpp' \) | wc -l
)"
lineCount="$(
    find "${sourceDirectory}" -maxdepth 1 -type f \
        \( -name '*.h' -o -name '*.cpp' \) -print0 |
        xargs -0 cat | wc -l
)"
traceCount=0
for tracePath in "${labRoot}"/common/traces/seed-*.txt; do
    traceName="$(basename "${tracePath}" .txt)"
    seedText="${traceName#seed-}"
    seedNumber="$((10#${seedText}))"
    "${lessonDirectory}/run.sh" --seed "${seedNumber}" --name Learner | cmp - "${tracePath}"
    traceCount=$((traceCount + 1))
done

printf 'files=%s lines=%s traces=%s/20\n' "${fileCount}" "${lineCount}" "${traceCount}"
