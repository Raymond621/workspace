#!/usr/bin/env bash
# Idempotent Cloud Agent install for this repository.
# Recurring environment builds currently invoke this script; keep it
# successful even when there are no language manifests yet.
set -euo pipefail

echo "workspace install: no package manifests found; nothing to install"
exit 0
