#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
set -eu
cd "$(dirname "$0")/../.."
mkdir -p build/reggae
${CC:-cc} ${REGGAE_TEST_CFLAGS:--O1} -ffp-contract=off -w -Ibuild/gen -Ifirmware/src -o build/reggae/test tests/reggae_test.c -lm
for preset in 0 1 2 3; do
  for knob in -1 0 1 2 3; do
    build/reggae/test "$preset" "$knob" "build/reggae/p${preset}-k${knob}.wav" \
      "build/reggae/p${preset}-k${knob}.ppm"
  done
done > build/reggae/audio-report.txt
python3 - <<'PY'
import re
from pathlib import Path
lines=Path('build/reggae/audio-report.txt').read_text().splitlines()
for p in range(4):
    hashes=[re.search(r'hash=(\d+)',s).group(1) for s in lines if s.startswith(f'preset={p} ')]
    assert len(hashes)==5 and len(set(hashes))==5, (p,hashes)
print('PASS: four presets, sixteen knob variants: audible, finite, bounded, release to silence, distinct audio; preset recall.')
print(Path('build/reggae/audio-report.txt').read_text())
PY
