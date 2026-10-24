#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
set -eu
cd "$(dirname "$0")/../.."
mkdir -p build/reggae
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/test tests/reggae_test.c -lm
for preset in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14; do
  for knob in -1 0 1 2 3; do
    build/reggae/test "$preset" "$knob" "build/reggae/p${preset}-k${knob}.wav" \
      "build/reggae/p${preset}-k${knob}.ppm"
  done
done > build/reggae/audio-report.txt
python3 - <<'PY'
import re
from pathlib import Path
lines=Path('build/reggae/audio-report.txt').read_text().splitlines()
for p in range(15):
    hashes=[re.search(r'hash=(\d+)',s).group(1) for s in lines if s.startswith(f'preset={p} ')]
    assert len(hashes)==5 and len(set(hashes))==5, (p,hashes)
print('PASS: fifteen presets, sixty knob variants: audible, finite, bounded, release to silence, distinct audio; preset recall.')
print(Path('build/reggae/audio-report.txt').read_text())
PY
python3 tools/reggae/check-protected.py
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/extended tests/reggae_extended.c -lm
for preset in 4 5 6 7 8 9 10 11 12 13 14; do
  build/reggae/extended "$preset" "build/reggae/extended-$preset.wav"
done > build/reggae/extended-report.txt
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/graphics tests/reggae_graphics.c -lm
build/reggae/graphics
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/style tests/reggae_style_demo.c -lm
for preset in 4 7 8 9 12; do
  build/reggae/style "$preset" "build/reggae/style-$preset.wav"
done > build/reggae/style-report.txt
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/siren-test tests/reggae_siren.c -lm
build/reggae/siren-test

${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/arranger tests/reggae_arranger.c -lm
build/reggae/arranger
