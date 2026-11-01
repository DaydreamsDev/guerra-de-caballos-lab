#!/usr/bin/env bash
set -euo pipefail

scriptDirectory="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
exec python3 "${scriptDirectory}/code/main.py" "$@"
