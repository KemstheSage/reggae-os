# Test evidence

Development host: macOS x86-64; Apple Clang 16; Emscripten 4.0.15; JieLi pi32v2
compiler through a Linux Docker container. Source baseline: Felucca v1.6.1
(`a53ecf4`), engines `c46e0086f69bdc26df1cd144dca9e683cc4531dd`.

## Confirmed baseline

- Original firmware application and `.fwsc` package built; upstream linker/MMIO checks passed.
- Original WebAssembly emulator built; upstream `emu_test.mjs` passed.
- Original standalone DSP regression: **98 golden renders unchanged**, zero health,
  voice/routing or crash failures. **58 CPU entries exceed the upstream instruction
  budget on this host.** This is a baseline failure, not a REGGAE-OS pass; thresholds
  and golden files were not rewritten to hide it.
- Full upstream runner was invoked before and after obtaining the target package.
  Its first attempt stopped because the prerequisite package was unavailable.
  Later baseline and modified full-suite runs are tracked in the evidence logs;
  do not infer a full-suite pass from the focused checks below.

## REGGAE-OS focused tests

`sh tools/reggae/test.sh` compiles the actual firmware emulator natively, with its
real audio ISR, virtual panel and simulated NOR storage. Each sound/knob variant
runs in a fresh process. It writes stereo WAV and 240 × 240 PPM evidence into
`build/reggae/` and checks:

- thirteen presets selected through the PRESETS encoder;
- each assigned encoder changes the firmware parameter;
- all fifty-two knob variants differ from their default and each other, with identical note timing;
- finite, non-silent stereo output, peak below 0.99 full scale;
- default-preset tail energy below 0.02 after release; large effect/envelope variants
  may have longer tails and are checked for eventual voice release;
- repeated triads release all voices;
- saved parameter values survive a change and user-preset recall.

`node web/emu/emu_test.mjs build/emu/felucca.wasm` checks the WebAssembly firmware:
boot, keys, MIDI, screen, LEDs, panel save across fresh instances, deterministic
audio and a heavy mixed-engine song. A native `native_check.c` render can be passed
as its second argument to compare the samples exactly.

The browser panel is inspected manually in the Codex in-app browser. A started
AudioWorklet plus nonzero offline audio establishes synthesis output, but does not
establish physical speaker quality or musical authenticity. Native evidence images
come from the real renderer, not a separate mockup.

## Compatibility labels

**CONFIRMED:** architecture, source mappings, shared C DSP/rendering, successful
upstream and modified build checks, focused test results when recorded.

**COMPILES:** pi32v2 hardware target. No physical installation has occurred.

**EMULATOR TESTED:** WebAssembly tests and desktop browser interaction. This does
not cover the actual USB/TRS electronics, DAC, panel scan or display bandwidth.

**HARDWARE UNTESTED:** flashing/recovery, real audio latency/underruns, high-polyphony
CPU behaviour, electrical MIDI, real knob mappings, battery operation and sampling
levels into EP-40 / Sidekick. Instrument gain and authenticity need musician audition.

## Safari and mobile limitations

The emulator resumes audio in the Switch on gesture and uses the existing touch
panel and pointer cancellation paths. It requires a 44.1 kHz AudioContext; an
unsupported rate now gives an error rather than knowingly playing at the wrong
pitch. Actual iPhone Safari is untested. Use HTTPS for a remote host.
[WebKit has documented rate/AudioWorklet regressions](https://bugs.webkit.org/show_bug.cgi?id=274507);
do not extrapolate desktop operation to every iOS version. Backgrounding, device
route changes, multitouch chords and interruption/resume require device tests.

## Prototype verification completed, 15 October 2026

- Hardware application: **513,808 bytes**. RAM data/BSS **61,000 / 73,728**;
  pool **328,896 / 368,640**. Target linker, RAM-call and MMIO checks passed.
  Installable package **610,090 bytes**, original FM-1_900 identity.
- Focused audio: all **20 cases passed** (four original settings plus sixteen
  single-knob variants). Measured peaks 0.09578–0.27161 full scale; release tails
  were zero in the measured final window. Preset storage and repeated note releases passed.
- `node tests/reggae_wasm.mjs`: all 20 renders matched native PCM exactly,
  **176,384 stereo frames per case**. Upstream emulator suite also passed,
  including its independent 88,200-frame native comparison.
- Full baseline and modified host runners finished, **neither passed overall**.
  Baseline failures include font spacing and host-dependent cost limits for FOLD,
  performance effects, reverb, PHYS and single-core regression budgets. The modified
  golden regression retained all 98 original renders unchanged and checked four
  appended presets, with no health, voice/routing or crash failures.
- Initial modified runs exposed a legacy default-table regression. Boot choices
  were separated from the migration table; project-format and motion/storage
  suites then passed on rerun. Preset-count assumptions in UI tests and editor
  mock tables were also updated; their rerun logs are preserved with the evidence.
- Real desktop browser started the AudioWorklet and displayed the firmware organ
  screen. A 390×844 desktop viewport check passed layout inspection only.

Local raw logs and sound/screen evidence are retained in `build/reggae/evidence/`
and `build/reggae/`. Host instruction counts are not device timing measurements.
No claim of a completely passing upstream suite, verified musical authenticity,
iPhone operation or physical hardware operation is made.

## Sound bank 0.2 — 15 October 2026

All 20 focused native audio cases passed after the registration/envelope revisions,
and all 20 matched WebAssembly PCM exactly (176,384 stereo frames each).
Fixed MIDI renders confirm sustained organ amplitude and decaying bass: RMS at
700–900 ms divided by RMS at 30–130 ms is 1.002 / 0.963 for the two organs,
0.226 for Deep Roots and 0.0079 for Digital Bass. These ratios establish envelope
behaviour, not similarity to the user's recordings. Raw measurements are in
build/reggae/articulation-v02.json. Original musical audition WAVs were regenerated.
The full upstream suite has not been rerun for this bank-only revision.

The 0.2 hardware build also passed the linker/RAM-call/MMIO checks: application
513,808 bytes, data/BSS 61,000/73,728, pool 328,896/368,640; package 610,090 bytes.
Physical hardware remains untested. Source commit ae87707; deployed Pages commit
35b12f0. The public page identifies sound bank 0.2 and versioned WASM loading avoids
reusing the earlier cached binary. Save files remain in the same browser storage.

## Bank 0.3 verification — 16 October 2026

- Native firmware: 13 defaults + 52 knob variants passed finite/non-silent/bounded
  PCM, encoder changes, repeated-note voice release and NOR parameter recall.
  FM6 recalled operator-patch bytes also match.
- `node tests/reggae_wasm.mjs`: all 65 cases produce bit-identical native/WASM
  16-bit stereo PCM (176,384 frames each).
- `tests/reggae_extended.c`: all nine additions produce audible eight-strike
  auditions; each of the eight Roots Kit lanes is exercised. Voices release.
- Approved organ eight-second WAV remains byte-for-byte identical to
  `assets/reggae-reference/roots-organ-approved.wav`.
- Upstream WebAssembly regression passes, including MIDI, panel, save, screen,
  deterministic rendering and the heavy mixed-engine song. Desktop execution time
  is not a hardware CPU measurement.
- Web editor regression passes using freshly generated firmware descriptors:
  `FELUCCA_DESC=build/reggae/desc-v03.json node web/test_web.mjs`.
  Assertions were updated for the appended presets/palette while preserving the
  original preset ordering, sample-set aliases and factory pattern defaults.
- Native 240 × 240 images and desktop browser screens inspected. Hardware builds
  pass linker/MMIO checks; no physical FM-1 or iPhone Safari test has occurred.

Reproduce extended auditions by running `sh tools/reggae/test.sh`; WAVs are in
`build/reggae/extended-4.wav` through `extended-12.wav`. The packaging script gives
these descriptive names under `auditions/`. Visual waveform/coil mechanisms are
parameter-linked illustrations, not evidence of acoustic authenticity.

## UI 0.3.1 — 16 October 2026

- Native graphics test passes: real rotor motion, ANIM OFF, parameter focus from
  panel input, 1.2-second expiry, and cancellation on a preset change. Native
  screenshots were inspected at 240 × 240; units use a supported smaller font.
- All 65 WebAssembly renders match the bank 0.3 native PCM references exactly;
  no sound or DSP changes. Upstream emulator regression passes.
- Desktop browser verified startup, musical labels, control colours and focused
  parameter view through the actual virtual KNOB 1 input.
- Final hardware build passes linker/MMIO checks: application 518,524 B;
  data+BSS 61,032 / 73,728 B; pool 328,896 / 368,640 B.
- Hardware timing and iPhone Safari remain unverified. Research used official
  manuals/static artwork, not hands-on testing of TE hardware.

## UI 0.3.2 — 16 October 2026

- Native graphics tests pass under AddressSanitizer, including clipped primitives,
  symmetric antialiased circle edges, animation and parameter focus. All thirteen
  native 240 × 240 screens inspected.
- All 65 native/WASM audio comparisons remain bit-identical. Upstream emulator
  regression passes. WASM build: 799,011 bytes.
- Hardware build passes linker/MMIO checks: application 519,948 B; data+BSS
  61,032 / 73,728 B; pool 328,896 / 368,640 B.
- Desktop WASM benchmark, median of three runs rendering ten seconds with a held
  note (before → after, milliseconds): organ 135.1 → 191.6; bass 168.7 → 175.4;
  siren 129.5 → 143.4; kit 83.4 → 165.6. These include audio and UI work, and show
  added rendering cost. They do not establish physical FM-1 real-time headroom.
- Physical display performance, DAC audio and iPhone Safari remain untested.

## UI 0.3.3 — 20 October 2026

- AddressSanitizer native graphics checks pass: mouth opens with audio level,
  rests with ANIM OFF, plus existing focus/rotary and edge-clipping checks.
- Rest/playing screenshots inspected at the native 240 × 240 resolution.
- All 65 WASM/native PCM comparisons remain bit-identical. Upstream emulator
  regression passes. WASM size: 800,135 bytes.
- No DSP or preset changes. Physical FM-1 timing and iPhone Safari remain untested.
- Hardware build passes linker/MMIO checks: application 520,240 B; data+BSS
  61,032 / 73,728 B; pool 328,896 / 368,640 B. COMPILES, not physically tested.

## UI 0.3.4 — 20 October 2026

- Native graphics tests pass under AddressSanitizer: melodica mouth/audio-level
  response, ANIM OFF, all four FX amount responses, and existing focus/rotary and
  primitive clipping checks. Real FX pages and eight redesigned screens inspected
  at native 240 × 240 resolution.
- All 65 native/WASM PCM comparisons remain bit-identical. Upstream emulator
  regression passes; WASM 803,173 bytes. No DSP or sound-bank edits.
- FM-1 build passes linker/MMIO checks: application 521,276 B; data+BSS
  61,032 / 73,728 B; pool 328,896 / 368,640 B. COMPILES.
- Physical FM-1 audio/display timing and actual iPhone Safari remain untested.
