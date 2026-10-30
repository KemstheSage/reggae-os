# UI 0.7.3 — individual note velocities

Adds NOTE VELOCITY to the SEQ pages after AUTOMATION. K1 selects a step, K2 a
chord-note slot or drum lane, K3 velocity 1–127, K4 STEP inheritance or explicit
NOTE. Gold bars identify the selected note. PRESETS also moves the step cursor.
The native 240 × 240 screen and all controls use the shared firmware path.

An explicit value overrides that note's step/drum accent; other notes retain their
own values or the shared step velocity. STEP returns to the previous behaviour.
The existing DETAIL VEL remains the shared value. This adds manual editing, not
a redesign of live-recording velocity capture. Save complete patterns via PROJECT;
user sound presets do not preserve these sparse note edits.

## Storage and playback

Reserved IDs 116–127 in the existing bounded record pool represent four note
slots and eight drum lanes. They are never indexed as sound parameters. Parameter
automation skips them; note playback reads them independently of automation ON/OFF,
including ratchets and the current arrangement source. Clearing sound automation
preserves them. Pattern clear/replacement and note deletion remove affected edits;
remaining melodic notes retain their values. Whole-pattern reorder/doubling and
undo use the existing record-copy paths. No flash offsets or sector sizes changed.

The 128-record capacity is shared with parameter automation/locks across all four
tracks. Full capacity refuses a new individual value with a visible message;
existing records can still be edited or removed. Inherited notes use no entries.

Projects containing reserved velocity records use FUNB, with the same 3840-byte
layout and checksum as FUNA. Old projects remain readable; ordinary projects
without individual edits still write FUNA. Reserved records disguised as FUNA are
rejected. Firmware before UI 0.7.3 cannot load FUNB projects. Preserve older project
backups before downgrading. Explicit values must be locked records with velocity
1–127; invalid data and duplicate IDs are rejected.

## Evidence

- Full REGGAE regression passes: 75 preset/control audio renders, protected sounds,
  sequencer/recording/arrangement, mixer audio, 532 screen renders, settings/save,
  extended chords, AUTO voice leading and individual velocities.
- New native tests pass, also with AddressSanitizer: actual voice velocities,
  audible output, physical-control selection/edit/inherit, track/note isolation,
  undo, doubling/halving, delete/clear, full-pool refusal, invalid record rejection,
  FUNB recall, FUNA spoof rejection and independent arrangement-source values.
- WASM test writes a chord through physical EDIT+keys, enters NOTE VELOCITY and
  changes each note with K1–K4. Measured energy: baseline 846.60, one softened note
  562.02, all softened 0.084, restored inheritance 814.60. Samples finite/unclipped.
- Final WASM AUTO, step gestures, baseline and heavy-song tests pass.
- Existing project-format and FUN10 tests pass. Legacy motion_test has three UI
  failures, identical on the unchanged UI 0.7.2 baseline 9a67ddf: old EDIT-clear,
  playing-song lock and EDIT-delete expectations. That suite is not claimed green.
- Guide regenerated: 102 screenshot pages, plus populated note/drum examples.
- Hardware build: app 545,752 bytes; static RAM 61,032 / 73,728; pool
  328,896 / 368,640. Linker, RAM-code and register-access checks pass.
- Actual package passes the address/undefined-behaviour-sanitized OTA simulator.

Package: `build/note-velocity/reggae-os-ui-0.7.3.fwsc`, 610,090 bytes.
SHA-256: `2052f19fa55fc6ebc1d19751cd4abad719e7628d70df7910988a38819e8e517a`.
USB identity FM-1_900; verify UI 0.7.3 on the device to distinguish this build.
**COMPILES / EMULATOR TESTED; HARDWARE UNTESTED.** No physical flash in this turn.
