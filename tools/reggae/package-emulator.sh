#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
set -eu
cd "$(dirname "$0")/../.."
test -f build/emu/felucca.wasm
cp LICENSE LICENSING.md CREDITS.md build/emu/
cp -R LICENSES build/emu/
# Include the submodule's actual source, not only its gitlink. Build tools included.
tar --exclude=.git --exclude=__pycache__ -czf build/emu/reggae-os-source.tar.gz \
  firmware tools tests assets web .github .gitmodules build.sh LICENSE LICENSES ./*.md
