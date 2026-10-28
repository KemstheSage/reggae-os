# UI 0.7.2 — automatic chord voice leading

Adds AUTO after the five existing voicings, preserving their saved numeric IDs.
On SCL → CHORD GARDEN, K3 chooses a chord and K4 chooses AUTO. Use a POLY
instrument. First chord: close position; later chords: nearest candidate inversion
using ordered voice motion, with played-register distance as the tie breaker.
Each track remembers its own previous live chord. An octave-or-larger root jump
starts a new register. Playing another voicing/single notes or track panic/reset
clears this transient memory. Preset/project loading uses the existing panic path.

The search is bounded to at most 20 candidates, four notes each, integer-only,
without heap allocation. It runs on new chord input, not per sample. Preview
rendering remains side-effect free. Existing key/MIDI note ownership retains the
chosen notes for release even when chord settings change while held. Recording
stores actual notes; existing sequences are not automatically revoiced. This is
local voice leading, not predictive harmony or classical counterpoint.

## Verification

- Focused native test: C–F–G–C inversion choices; track and preview isolation;
  register reset; all chord types across MIDI range retaining pitch classes and
  note count; overlapping MIDI releases, finite audible output and project save.
- Legacy chord tests pass, including keyboard, MIDI, recording, ARP and kit paths.
- WASM actual SCL/K4 input: first AUTO chord matches CLOSE audio, subsequent
  chords differ, repeated performances are deterministic, no clipping/non-finite
  output, release returns to silence. Native 240 × 240 framebuffer inspected.
- Existing 15 extended-chord WASM tests and baseline/heavy-song emulator tests pass.
- Full REGGAE regression suite passes: 75 preset/control audio renders, 40
  protected organ/piano/bass renders unchanged, recording/arrangement/step editing,
  mixer pan/reverb audio, 518 screen captures, settings/naming/save, existing
  extended chords and new AUTO keyboard/MIDI paths.
- Hardware build passes linker, RAM-code and register-access checks: application
  542,832 bytes; static RAM 61,032 / 73,728; pool 328,896 / 368,640. Relative to
  UI 0.7.1: +236 application bytes, +32 static RAM bytes (including build alignment).
- Actual package passes the OTA simulator with address/undefined-behaviour sanitizers.

Hardware package: `build/voice-leading/reggae-os-ui-0.7.2.fwsc` (610,090 bytes).
SHA-256: `9ee2eddb8af922492cbd78e6d6d7e02958dacfb3ce31ce4192815c401c7b717e`.
USB identity remains FM-1_900; distinguish releases using the on-screen UI version.
**COMPILES / EMULATOR TESTED; AUTO is HARDWARE UNTESTED.** The earlier physical
installation was UI 0.7.1. This development turn does not flash the device.

Sources: `firmware/src/chord.c`, `params.c`, `reggae_workspace.c`; new native/WASM
voice-leading tests, regression runner, emulator version, README, SEQUENCER and
screenshot guide. No synthesis engines or preset sounds changed.
