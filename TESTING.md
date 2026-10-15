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

- four presets selected through the PRESETS encoder;
- each assigned encoder changes the firmware parameter;
- all sixteen variants produce distinct audio, with identical note timing;
- finite, non-silent stereo output, peak below 0.99 full scale;
- tail energy below 0.001 after release;
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
