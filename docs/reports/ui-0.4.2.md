# UI 0.4.2 — restored editor, add / replace / clear

The UI 0.4.0 piano roll and full drum grid are restored. Simple pads from 0.4.1
are removed. Existing arrangement screens remain.

On stopped melodic STEP only:

- Hold EDIT and play the physical/emulated keys: add unique pitches to this step.
- Hold REC and play: first valid key replaces old notes; further keys in the same
  hold form the chord. Holding REC without playing does not erase anything.
- EDIT + REC together clears notes and locks. SAVE held undoes the operation.
- Cursor stays on the edited step. Quick REC tap still arms/disarms recording;
  EDIT alone does not erase. Up to four notes per step remains the existing limit.

Modifier release is consumed so it cannot select an engine, open a settings
layer or arm recording after a note-edit gesture. Notes remain audible while
entering them. Releasing the modifier before keys does not move the cursor.

The shortcuts use physical keyboard edges; incoming MIDI notes retain existing
behavior. Drum-grid, DETAIL, live playback/recording and other-page mappings are
unchanged. EDIT/REC quick-setting layers remain available outside this context.

Files: ui.c (scope), ui_input.c (gesture and undo), ui_layer.c (keyboard routing),
reggae_seq_ui.c (restored screen/hints), emulator version, tests and documentation.
No DSP, patch definitions, stored project format or partition changes.

Validation results and target sizes: TESTING.md. Physical FM-1 and iPhone Safari
remain untested. Public URL: https://kemsthesage.github.io/reggae-os/?ui=0.4.2
