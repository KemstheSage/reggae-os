# UI 0.5.1 — groove roll and whole-roll clear

The existing workflow remains. No further SLOOP features are added in this release.

On stopped melodic STEP: hold EDIT, then hold SEQ for one second to clear all
64 steps and their parameter locks on the selected track. Early release cancels.
Sound parameters, length, tempo and other tracks remain; hold SAVE to undo.
The combined button releases are consumed so they cannot open another SEQ page.
EDIT+REC retains its single-step clear behavior. Drum grid and playback are excluded.

SEQ gains a 45-label header, beat-driven spindle, beat lights, warm note bars,
green lanes, red accents and cabinet-style parameter wells. Existing roll/grid
geometry and all musical marks remain. The arrangement page uses matching trim.
The actual 240×240 firmware renderer provides these graphics in WASM and on target;
no additional framebuffer, audio processing or browser-only graphics are introduced.
Animation remains capped at 10 Hz; spindle motion respects animations-off.

Files: reggae_seq_ui.c, ui_input.c, native/WASM step tests, emulator version and
project documentation. Test/build results: TESTING.md. Hardware and iPhone Safari
remain untested. Access: https://kemsthesage.github.io/reggae-os/?ui=0.5.1
