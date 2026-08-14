#!/usr/bin/env bash
#
# Idempotent environment bootstrap for the MM32F0144C6P firmware project.
#
# Installs the GNU Arm Embedded toolchain (arm-none-eabi) plus make, then
# builds the firmware to prove the toolchain and vendored device library are
# in a working state. Safe to run repeatedly.
set -euo pipefail

# Resolve the repository root (parent of this script's .cursor directory).
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

echo "==> Installing ARM cross-toolchain and build tools"
export DEBIAN_FRONTEND=noninteractive
sudo apt-get update -qq
sudo apt-get install -y --no-install-recommends \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    make

echo "==> Toolchain versions"
arm-none-eabi-gcc --version | head -n 1 || true
make --version | head -n 1 || true

echo "==> Building firmware"
make clean
make

echo "==> Build artifacts"
ls -l build/*.elf build/*.bin build/*.hex
