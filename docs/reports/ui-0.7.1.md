# UI 0.7.1 — expanded chords and screenshot guide

Adds 15 append-only chord IDs: SUS2, MAJ6, MIN6, ADD9, mADD9, DOM9,
MAJ9, MIN9, 7SUS4, DIM, DIM7, m7b5, AUG, mMAJ7 and 6/9.
Old IDs and existing instrument presets remain unchanged. Ninth and 6/9 shapes
omit the fifth to fit four voices. Their +OCT voicing substitutes the lower root
for the upper root, preserving the colour tones. Eight voices remain shared.

SCL → CHORD: K1 root, K2 scale, K3 type, K4 voicing. A polyphonic sound is
required for complete chords. Drum kits ignore chord mode. Projects save these
settings; loading a user sound preserves the track's current chord configuration.

The [quick guide](https://kemsthesage.github.io/reggae-os/guide/) contains 101
native 240×240 firmware screenshots, navigation, page purpose, usage and K1–K4
assignments. It includes all visible PAGES entries, all 13 active engine EDIT
variants, curated sound screens, mixer modes, held layers, settings and dialogs.
Conditional insert/automation controls are explained within their page entries.
Crash diagnostics and bootloader internals are outside this musician's guide.

## Verification

- Native AddressSanitizer chord test: 15 expected interval/name sets, 9,600
  range/voicing combinations, actual MIDI audio, releases after chord changes,
  extended bass voicing and project save/load passed.
- Existing chord regression passed (legacy shapes and MIDI/arp/release behavior).
- WASM physical SCL/K3 inputs: all 15 added choices produce distinct, finite,
  non-silent, unclipped audio; release after changing shape decays below 0.001.
- All 75 existing native/WASM preset/control PCM comparisons passed unchanged.
- Emulator core/heavy-song and sequencer-menu regressions passed.
- Screenshot generator validates all images as 240×240 and all entries have
  four descriptions. Guide preview checked at 390px width; search exercised.
- Hardware target build: image 542,596 bytes; static RAM 61,000 / 73,728;
  pool 328,896 / 368,640. Linker and register-access checks passed.
- Hardware and physical iPhone Safari remain untested. Browser-width preview
  is not a claim of testing on an iPhone.

## Files and reproducibility

Chord implementation: `firmware/src/chord.c`, `params.c`.
Guide capture: `tests/reggae_guide.c`; generator: `tools/reggae/build-guide.py`;
published assets: `docs/guide/`. Tests: `tests/reggae_chords.c` and
`tests/reggae_chords_wasm.mjs`. CI runs both and regenerates the guide.
Emulator packaging copies the guide and exposes a footer link.

After generating the normal firmware tables, rebuild the guide with:

```sh
mkdir -p build/reggae/guide
cc -O0 -ffp-contract=off -w -Ibuild/gen -Ifirmware/src \
  -o build/reggae/guide-capture tests/reggae_guide.c -lm
build/reggae/guide-capture
python3 tools/reggae/build-guide.py  # requires Pillow
```

Next validation priority: musicians auditioning the new chord voicings within
four-track arrangements, then physical-device voice-load and control checks.
