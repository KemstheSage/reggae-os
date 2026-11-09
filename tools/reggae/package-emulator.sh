#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
set -eu
cd "$(dirname "$0")/../.."
test -f build/emu/felucca.wasm
# Refresh the wrapper when only HTML changed after the WASM build.
sed -e 's/@VERSION@/v1.6.1/g' -e 's/@PREVIEW@/ (preview)/g' web/emu/index.html > build/emu/index.html
cp web/emu/worklet.js build/emu/
cp LICENSE LICENSING.md CREDITS.md SEQUENCER.md build/emu/
cp -R LICENSES build/emu/
# The checked-in guide is generated from shared-firmware framebuffer captures.
mkdir -p build/emu/guide
cp -R docs/preview-0.8.2/. build/emu/guide/
# Include the submodule's actual source, not only its gitlink. Build tools included.
tar --exclude=.git --exclude=__pycache__ -czf build/emu/reggae-os-source.tar.gz \
  firmware tools tests assets web docs .github .gitmodules build.sh LICENSE LICENSES ./*.md

# Optional original musical auditions, rendered by tests/reggae_demo.c.
if test -f build/reggae/roots-organ.wav; then
  mkdir -p build/emu/auditions
  cp build/reggae/roots-organ.wav build/reggae/rocksteady.wav \
     build/reggae/deep-roots-bass.wav build/reggae/digital-bass.wav build/emu/auditions/
fi
# Extended audition phrases use the same firmware, including each drum lane.
mkdir -p build/emu/auditions
i=4
for name in reed-melodica warm-tine piano-chop brass-stabs muted-skank dub-siren round-fm-bass dub-drawbars roots-kit jungle-signal fast-alarm; do
  if test -f "build/reggae/extended-$i.wav"; then
    cp "build/reggae/extended-$i.wav" "build/emu/auditions/$name.wav"
  fi
  i=$((i + 1))
done
# Musical audition candidates (original MIDI studies) and controlled old phrases.
for entry in 4:reed-melodica 7:brass-stabs 8:muted-skank 9:dub-siren 12:roots-kit; do
  number=${entry%%:*}
  name=${entry#*:}
  if test -f "build/reggae/style-$number.wav"; then
    cp "build/reggae/style-$number.wav" "build/emu/auditions/$name-style.wav"
  fi
  if test -f "build/reggae/before-bank04/extended-$number.wav"; then
    cp "build/reggae/before-bank04/extended-$number.wav" "build/emu/auditions/$name-before-bank04.wav"
  fi
done

# Clear name for the sampled replacement; retain brass-stabs.wav for existing bookmarks.
cp build/emu/auditions/brass-stabs.wav build/emu/auditions/roots-trumpet.wav

python3 tools/reggae/percussion-pack.py
mkdir -p build/emu/percussion
cp build/reggae/percussion/* build/emu/percussion/
cp assets/samples-cc0/REGGAE_PERC/ATTRIBUTION.md build/emu/percussion/

python3 tools/reggae/gen-roots-drums.py
mkdir -p build/emu/drums
cp build/reggae/drums/RDMDRM2.slot build/emu/drums/
cp assets/samples-cc0/ROOTS_DRUM/ATTRIBUTION.md build/emu/drums/
cp docs/drums-preview/index.html build/emu/drums/
for file in build/reggae/drums/*.wav build/reggae/drums/kit-*.png; do
  if test -f "$file"; then cp "$file" build/emu/drums/; fi
done
