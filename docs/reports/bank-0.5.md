# Bank 0.5 — roots trumpet and dub/jungle sirens

21 October 2026. Shared C firmware and WASM; UI art remains 0.3.4.

## Changes

- ROOTS TRUMPET replaces the curated synth-brass entry (bank index 7): three
  CC0 VSCO 2 CE recordings, 22,050 Hz mono IMA ADPCM, tuned zones, looped sustain,
  tone/attack/decay/release. Original analog brass remains in engine browsing.
- DUB WAIL (index 9): sine oscillator, continuous triangle pitch modulation, echo.
- JUNGLE SIGNAL (index 13): filtered pulse, descending pitch envelope and zero
  sustain; controls for fall, length, echo send and tone.
- FAST ALARM (index 14): filtered pulse with square pitch modulation; range, rate,
  echo send and tone. All use the existing animated siren illustration.
- Existing user sample IDs 5–7 stay fixed. New ROM sample ID 8 works in SAMPLE,
  GRAIN and waveform display. All sample additions are in flash, not new RAM buffers.
- No organ, piano, bass, melodica, guitar, drum, global effects or artwork redesign.

## Verification

- 75 native audio cases: fifteen presets, defaults plus four control variations;
  non-silent, finite, bounded output, note release and saved-preset recall.
- All 75 native renders match WASM PCM byte-for-byte.
- All 40 protected organ/piano/bass renders match the pre-change hashes.
- SAMPLE tests: trumpet actual sample selection, sustain and pitch over MIDI 60–79;
  GRAIN source selection; original piano zones, pitch and duration; legacy PIANO HD
  in USR1 still matches the built-in piano at twelve notes byte-for-byte.
- Existing emulator regression passes, including panel, storage and multi-engine audio.
- Native graphics checks pass at 240 × 240: fifteen screens, animation off, parameter
  feedback and effect illustrations. Images generated in build/reggae/screen-*.png.

- Held-key Jungle Signal test passes: four retriggers, silence while held and
  voice release after note-off. Graphics pass with AddressSanitizer.
- Final FM-1 build: app 553,340 / 581,564 bytes; data+BSS 61,032 / 73,728;
  pool 328,896 / 368,640. Package 610,090 bytes, FM-1_900. WASM 834,312 bytes.
  Hardware is COMPILES / HARDWARE UNTESTED. See TESTING.md for commands.

## Source and limits

Samples: assets/samples-cc0/BRASS/PROVENANCE.json pins upstream revision, full-file
SHA-256 and CC0 license. No commercial song audio is distributed. The renderer
and DSP run in firmware; the browser only presents virtual hardware.

This is one acoustic trumpet, not a multi-player horn section. Natural timbre is
best in the tested C4–G5 range; extreme transposition remains artificial. The
siren signals are original synthesis, not copies of named jungle samples.
Automated tests do not establish musical authenticity or subjective sound quality.
A musician's audition remains necessary. Physical FM-1 and iPhone Safari are untested.

## Access

https://kemsthesage.github.io/reggae-os/?bank=0.5
Tap Switch on, use PRESETS from Roots Organ; ROOTS TRUMPET is eighth. The two
new sirens follow ROOTS KIT. Existing notes, controls and SAVE workflow apply.
Auditions: /auditions/roots-trumpet.wav, /auditions/dub-siren.wav,
/auditions/jungle-signal.wav and /auditions/fast-alarm.wav.
