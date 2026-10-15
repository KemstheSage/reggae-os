#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
set -eu
cd "$(dirname "$0")/../.."
test -f build/emu/felucca.wasm
cp LICENSE LICENSING.md CREDITS.md build/emu/
cp -R LICENSES build/emu/
# Include the submodule's actual source, not only its gitlink. Build tools included.
tar --exclude=.git --exclude=__pycache__ -czf build/emu/reggae-os-source.tar.gz \
  firmware tools tests assets web docs .github .gitmodules build.sh LICENSE LICENSES ./*.md

# Optional original musical auditions, rendered by tests/reggae_demo.c.
if test -f build/reggae/roots-organ.wav; then
  mkdir -p build/emu/auditions
  cp build/reggae/roots-organ.wav build/reggae/rocksteady.wav \
     build/reggae/deep-roots-bass.wav build/reggae/digital-bass.wav build/emu/auditions/
fi
