# UI 0.4.0 — first sequencer and song-workflow pass

24 October 2026. Sound bank remains 0.5.

## Delivered

- Native STEP note pattern / eight-lane drum grid in the instrument-screen layout.
  Existing roll/grid rendering, notes, ties, locks and playhead are reused.
- ARRANGE: four visible section rows, all four track pattern choices, mute cells,
  repeats, selected-cell highlight, missing-source hint and + ADD row. All sixteen
  sections are accessible. View follows the running section across pages.
- Labelled physical knobs and transport/navigation hints. Existing GLO, SEQ,
  PLAY, REC, SAVE and ALGORITHM input conventions remain unchanged.
- Song-making guide: SEQUENCER.md, linked beside the public emulator.
- No new DSP, transport algorithm, file format, source slot or voice allocation.

The underlying song functionality already existed: four parts, four saved source
projects, sixteen sections with up to sixteen repeats, per-track source/mute and
64-step patterns. This release makes the core editing/arrangement screens clearer;
it does not claim that all workstation screens have been redesigned. DETAIL,
automation, mixer, saving and held-button tool layers are the next visual work.

## Validation

- New native test with AddressSanitizer exercises actual SEQ/GLO button events,
  step selection, add-section and repeat knob events, two audible song sections,
  automatic stop, saved arrangement recall, silent-lane persistence, final-row
  scrolling and drum hit editing. No sanitizer errors.
- WASM panel test verifies SEQ/GLO navigation and framebuffer changes from adding
  a section and editing its repeats.
- All 75 instrument/control renders in the new WASM match the existing bank-0.5
  native PCM. No sound-bank changes.
- Existing WASM emulator regression passes.
- Native screen captures inspected at 240 × 240: note editor, drum grid and arranger.
- Hardware build passes: app 556,380 / 581,564 bytes; data+BSS 61,032 / 73,728;
  pool 328,896 / 368,640. WASM 844,855 bytes. Hardware remains untested.

## Files

New firmware/src/reggae_seq_ui.c, integrated in ui_draw.c. New tests/reggae_arranger.c
and tests/reggae_seq_wasm.mjs; native test added to tools/reggae/test.sh. Emulator
version/cache key, packaging, README, roadmap, UI/testing docs and SEQUENCER.md.

## Access and next priority

https://kemsthesage.github.io/reggae-os/?ui=0.4.0
Tap Switch on, SEQ for pattern editing, GLO for arrangement. Existing saved
projects remain intact. Read SEQUENCER.md before composing across A–D sources.
Next: mixer, project-browser and automation workflow. Physical hardware timing,
iPhone Safari and musical usability require device/user validation.
