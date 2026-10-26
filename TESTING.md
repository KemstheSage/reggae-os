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

## Sound bank 0.4 — 21 October 2026

- Native 65-case defaults/knob suite passes: finite, non-silent, bounded audio,
  distinct knob outputs, repeated-note release and NOR preset recall.
- `python3 tools/reggae/check-protected.py`: all 40 organ/piano/bass render hashes
  match the pre-edit bank. This check is now part of tools/reggae/test.sh.
- Existing extended auditions exercise all eight drum lanes and all additions.
- Five new original 16-second performance studies pass finite/non-silent/unclipped
  audio and released-voice checks. Peaks: reed 0.08601, brass 0.24225, guitar
  0.07556, siren 0.11870, kit 0.26428 (full scale 1.0).
- Graphics regression passes. No renderer changes in this sound-only update.
- Numerical evidence is not subjective listening validation or proof of stylistic
  authenticity. Physical FM-1 and actual iPhone Safari remain untested.
- Final WASM/native parity passes all 65 cases; upstream emulator regression passes.
  WASM: 803,259 bytes. FM-1 linker/MMIO checks pass: application 521,344 B;
  data+BSS 61,032 / 73,728 B; pool 328,896 / 368,640 B. COMPILES.

## Sound bank 0.5 — 21 October 2026

- `REGGAE_TEST_CFLAGS=-O0 sh tools/reggae/test.sh`: 75 default/control audio
  renders, release, repeated notes and storage; extended auditions and fifteen
  native 240 × 240 screens. Graphics assertions pass.
- `node tests/reggae_wasm.mjs`: all 75 renders byte-identical to native PCM.
- `python3 tools/reggae/check-protected.py`: all 40 approved organ/piano/bass
  default/control renders match the pre-change hashes.
- `node web/emu/emu_test.mjs build/emu/felucca.wasm`: existing regression passes.
- `tests/sample_test.c` compiled at -O0: original piano and legacy USR1 playback,
  new sample-ID mapping, trumpet sustain and measured pitch across MIDI 60–79,
  and GRAIN source selection all pass. PIANO HD matches at twelve notes.
- `tests/reggae_siren.c`: four repeated Jungle Signal triggers; held key fades
  to silence, and note-off releases the voice. Also included in test.sh.
- `tests/reggae_graphics.c` compiled with AddressSanitizer: all checks pass.
- Final hardware build: app 553,340 / 581,564 bytes; data+BSS 61,032 / 73,728;
  pool 328,896 / 368,640. Package 610,090 bytes, identity FM-1_900. Existing HAL,
  register-access and interrupt/c1-text build guards pass. WASM 834,312 bytes.

Hardware status: COMPILES, HARDWARE UNTESTED. Emulator audio tested by directly
executing the actual WASM exports; iPhone Safari interaction remains untested.
The final test runs were individual commands as well as the main script; the
held-key siren check was added to the script after its successful standalone run.
No subjective listening comparison or physical CPU-headroom measurement is claimed.

## UI 0.4.0 — 24 October 2026

`tests/reggae_arranger.c` passes compiled with `-O0 -ffp-contract=off
-fsanitize=address`: native panel navigation, section creation/repeats, audible
multi-section playback, automatic stop, saved arrangement recall, last-row view
and drum hit editing. Test fixtures use the existing project writer; this is not
a claim that every SAVE gesture has been retested. Native screenshots inspected.

`node tests/reggae_seq_wasm.mjs` passes the corresponding WASM screen/navigation
smoke test. `node tests/reggae_wasm.mjs` confirms all 75 instrument/control PCM
renders unchanged from bank 0.5. Existing `web/emu/emu_test.mjs` passes.

Hardware app: 556,380 B; data+BSS 61,032 B; pool 328,896 B. Same memory limits;
25,184 B application space remains. WASM 844,855 B. Build guards pass. No physical
FM-1 or iPhone Safari test and no hardware CPU-budget measurement. Scope and
remaining screens are recorded in docs/reports/ui-0.4.0.md.

## UI 0.4.1 — simple pads, 24 October 2026

The expanded arranger test passes under AddressSanitizer, including selected-drum
lane changes and reaching step 64 through the physical encoder path. Existing
section creation/repeats, audible song playback, automatic stop, recall and drum
hit editing still pass. Native 240 × 240 note/drum screenshots inspected.

WASM navigation smoke test and existing emulator regression pass. All 75
instrument/control renders match bank-0.5 native PCM byte-for-byte. Hardware
build passes: app 557,280 / 581,564 B; data+BSS 61,032 / 73,728 B; pool
328,896 / 368,640 B. WASM 847,651 B. No new DSP or audio buffers.
Physical FM-1 and iPhone Safari remain untested.

## UI 0.4.2 — add / replace / clear, 24 October 2026

`tests/reggae_step_gestures.c` passes with AddressSanitizer: physical EDIT adds
and deduplicates notes, REC replaces with a chord, EDIT+REC clears, SAVE-hold
undo restores, cursor stays fixed, modifier-first release is safe, REC tap
still arms/disarms, and held REC without notes leaves the step intact. No stuck
voices or accidental engine changes were detected. The arranger regression also
passes under AddressSanitizer. Restored note-editor screenshot checked at 240 × 240.

`node tests/reggae_step_wasm.mjs` verifies audible added/replacement notes and
silent cleared-pattern playback in actual WASM. Its audio window spans more than
two complete default patterns so a short first-step note cannot fall outside the
measurement. WASM navigation/framebuffer tests and existing emulator regression
pass. All 75 instrument/control renders still match bank-0.5 native PCM exactly.

The new gestures apply to the stopped melodic STEP editor using physical/emulated
keys. MIDI entry, drum-grid controls and live recording retain existing behavior.
Physical FM-1 and iPhone Safari remain untested.

Final hardware build passes its guards: app 557,292 / 581,564 B;
data+BSS 61,048 / 73,728 B; pool 328,896 / 368,640 B.
WASM is 846,960 B. Hardware status: COMPILES, HARDWARE UNTESTED.

## UI 0.5.0 — first live-recording integration

`tests/reggae_live_record.c` passes under AddressSanitizer. It exercises physical
panel input: startup instruments, arm/cancel/rapid re-arm, first-note capture on
step zero, recording off while playback continues, bass overdub, track handoff,
T4 drum entry and HOME preset protection, stop/disarm, saved-project recall,
MIDI note-start, PLAY-start and waiting-arm cancellation on track change.
All voices release after the tests. The existing step-gesture and arrangement
regressions pass under AddressSanitizer. READY and RECORDING screenshots were
inspected at native 240 × 240 resolution.

Free-tempo capture and SLOOP's live song-section recording are not implemented.
The recording workflow uses existing quantisation and note-length/tie behavior;
it is not an audio looper. Physical FM-1 and iPhone Safari remain untested.

`node tests/reggae_live_wasm.mjs` passes actual-WASM audible organ/bass/drum
layering and clean-stop checks. Existing WASM step-entry and arrangement-screen
checks pass. All 75 sound-bank/control renders match the prior native PCM exactly;
`web/emu/emu_test.mjs` passes too.

Hardware build guards pass: app 558,024 / 581,564 B; data+BSS 61,048 / 73,728 B;
pool 328,896 / 368,640 B. WASM 846,993 B. This is COMPILES and EMULATOR TESTED,
not physically verified hardware performance.

## UI 0.5.1 — thematic SEQ and clear roll

The expanded `reggae_step_gestures.c` passes under AddressSanitizer: early release
cancels EDIT+SEQ, a one-second hold clears all 64 steps (including hidden step 64),
button releases keep the STEP page open, SAVE restores the whole roll, and sound
parameters/other tracks remain unchanged. Existing add/replace/single-step clear,
fixed cursor and released-voice checks pass. The native arrangement regression
also passes under AddressSanitizer. Piano-roll, drum-grid and arrangement graphics
were inspected together at 240 × 240 each; no labels or grid marks are obscured.

WASM tests pass for whole-roll clear/silence and audible undo, existing step
entry, arrangement navigation and live organ/bass/drum recording. All 75 sound
and knob renders remain byte-identical to bank-0.5 native PCM. The existing
emulator regression passes. Physical hardware and iPhone Safari remain untested.

Hardware build guards pass: app 559,180 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. WASM is 848,542 B. Status: COMPILES and EMULATOR TESTED.

## UI 0.5.2 — record character

`tests/reggae_seq_character.c` passes under AddressSanitizer: physical organ keys
change the character's pixels, release returns it to the same resting face,
physical drum keys trigger the same voice-activity response, and reduced-animation
mode retains mouth feedback. Idle and playing screenshots were inspected at the
native 240×240 resolution. Voice activity includes release tails and is measured
before the mixer, not as final output amplitude.

Hardware build guards pass: app 559,392 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. No added audio buffers. Physical FM-1 and iPhone Safari
remain untested.

WASM step/whole-roll clear+undo, arrangement and live-recording tests pass.
All 75 preset/control renders match bank-0.5 native PCM. Existing emulator
regression passes. WASM is 849,889 B, compiled with restored Emscripten 4.0.15.

## UI 0.5.3 — tabletop sequencer

Native arrangement/navigation/editing/audio/flash recall and record-character
regressions pass under AddressSanitizer. Piano-roll, drum-grid and arrangement
screens were inspected at their native 240×240 resolution. WASM step entry,
whole-roll clear and audible undo, arrangement controls and live layered recording
pass. All 75 preset/control PCM renders remain identical to bank 0.5. The upstream
emulator regression passes. No DSP or input mapping changes.

Hardware guards pass: app 560,696 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. WASM: 856,325 B. Grid inset uses a temporary 480-byte
stack row, no added framebuffer. COMPILES and EMULATOR TESTED; physical FM-1
and iPhone Safari remain untested.

## UI 0.5.4 — layout correction

The native character regression passes under AddressSanitizer, including a new
32-state check that every drawn character pixel stays within its header bay.
The native arrangement regression passes. Roll, drum-grid and arrangement
screens were inspected at 240×240: the character and text remain inside the case.
Long preset names, control values and messages use bounded text fitting; common
feel values use short labels (NORM, ACC, A+SL). Physical hardware and iPhone Safari
remain untested. No sound or input behaviour changes.

Final WASM step entry/clear/undo, arrangement and layered live-recording tests
pass. Hardware build guards pass: app 560,576 / 581,564 B; data+BSS 61,064 /
73,728 B; pool 328,896 / 368,640 B. WASM: 856,028 B.

## UI 0.5.5 — smaller instrument names

Rendered and visually inspected all 15 native performance screens at 240×240.
Category/preset names use AF_S (9-pixel capitals); no DSP or input changes.
The existing WASM emulator regression passes. Hardware guards pass: app 560,632 /
581,564 B; data+BSS 61,064 / 73,728 B; pool 328,896 / 368,640 B. WASM: 856,102 B.
Physical FM-1 and iPhone Safari remain untested.

## UI 0.5.6 — sequencer heading size

Rendered and inspected the native roll, drum grid and arrangement screens.
The native sequencer regression and WASM SEQ/GLO/navigation/control test pass.
Only the sequencer title font changes: AF_M to AF_S, leaving a clear gap above
tempo/status. WASM is 856,102 B. Physical FM-1 and iPhone Safari remain untested.
Hardware guards pass: app 560,632 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. No audio or input changes.

## UI 0.5.7 — compact parameter typography

Rendered and inspected the native roll, drum grid and arrangement at 240×240.
All four labels and values use the new XS face (10 px raster size, 8 px capital
band), centred within the existing panels. Native sequencer regression passes.
Font generation uses the existing Inter Tight OFL source and documented Debian
libraqm toolchain. No audio or input changes. Physical hardware and iPhone Safari
remain untested.

WASM SEQ/GLO/navigation/control regression passes. Hardware size guards pass:
app 564,388 / 581,564 B; data+BSS 61,064 / 73,728 B; pool 328,896 / 368,640 B.
WASM is 859,790 B. Font tables add flash data; RAM allocations are unchanged.

## UI 0.5.8 — sequencer menus and compact footer

Native menu and arrangement regressions pass under AddressSanitizer. The new
menu test checks physical SEQ navigation, detail chance editing, automation
playback toggle and OCT+ lock creation, held-SEQ length editing, Euclid hit
editing/cancel, cleanup selection and drum tool rendering. Native screenshots
cover roll, detail, automation/list/add/lock, melodic/drum tools, Euclid and cleanup.
The menu regression is included in the native test runner and WASM CI.

WASM menu navigation/knob responses, arrangement, step entry/clear/undo and live
layered recording pass. The existing emulator regression passes. Shared parameter
formatting is retained through a scoped card-render callback; editing/DSP code
is unchanged. Compact automation rendering retains units and inactive states.

Hardware guards pass: app 565,752 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. WASM: 862,403 B. Physical hardware and iPhone Safari
remain untested. Project/mixer/record settings and confirmation dialogs retain
their existing layout.

## UI 0.5.9 — remove hint bar

Native sequencer-menu and WASM menu/physical-knob regressions pass. Inspected
native 240×240 rendering: the compact hint sits directly on the case above its
bottom bevel, with no black background strip. No DSP or input changes.
Hardware guards pass: app 565,788 / 581,564 B; data+BSS 61,064 / 73,728 B;
pool 328,896 / 368,640 B. WASM: 862,134 B. Physical FM-1 and iPhone Safari
remain untested.

## UI 0.6.0 live performance

`tests/reggae_performance.c` (native ASAN) verifies four GLO level knobs, mute,
solo, FX gate/mono keys, root/voicing changes, audible chord and complete release.
`tests/chord_test.c` passes. `tests/perform_test.c` at -O2 passes functional
checks including added stereo gate, mono sum, silence and dry release. Its
harmonizer and flanger/phaser Mac instruction thresholds fail in both the
previous committed release and this revision; physical CPU load is unverified.
All 75 native/WASM PCM comparisons pass and 40 protected renders are unchanged.

`tests/reggae_performance_wasm.mjs` loads the optional pack into emulated NOR
and renders each sample through the actual SAMPLE engine. Five pieces have
nonzero energy, peaks 0.064–0.075 and silent tails; an unmapped note is silent.
`tests/reggae_percussion_upload.mjs` checks occupied-slot preservation, identical
pack reuse, invalid CRC rejection and persistence after flash reload.
Local browser testing confirms audio initialization, the upload success message
and the four-track mixer display. This is not a physical iPhone Safari test.
See `docs/reports/ui-0.6.0.md` for native screenshots and firmware-size results.

UI 0.6.1: native ASAN performance test verifies a normal HOME tap opens the
mixer, it stays open without any held button, four independent levels, mute,
solo clearance when leaving with a solo key held, selected-track pan on the
following page, and restored note keys after leaving. WASM sequencer-menu and
emulator regression pass. Hardware build and memory guards pass: 568,316-byte
image, 61,064-byte static RAM, 328,896-byte pool. Physical hardware untested.

UI 0.6.2: native ASAN panel regression adjusts pan/reverb on each of four tracks
using ALGORITHM and the physical knob paths. `tests/reggae_mixer_audio.c` checks
left/right channel energy after pan changes and a nonzero reverb tail versus a
silent dry tail. Both pass. WASM menu and emulator regressions pass. FM-1 image
568,720 bytes, static RAM 61,064 and pool 328,896; memory guards pass. Hardware
and iPhone Safari remain untested. See `docs/reports/ui-0.6.2.md`.
