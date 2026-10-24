# UI 0.4.1 — simpler pattern editing

24 October 2026. Sound bank 0.5 unchanged.

The main STEP screen now shows sixteen large pads (four by four). Drum editing
shows one named drum lane at a time instead of eight simultaneous lanes. Notes,
HIT, HOLD and chord + markers make the pattern readable without a piano roll.
The cream selection outline and green playback underline have distinct shapes;
accent and automation-lock markers remain. Steps beyond length are dim.

Page titles are MAKE A PATTERN, MAKE A BEAT and SONG. Footer hints explain the
next action. Four physical knobs and keyboard/recording behavior stay unchanged;
DETAIL and AUTOMATION remain available by tapping SEQ. This is a simplification
of the primary editor, not a claim that every secondary menu has been redesigned.
Four-track/sixteen-section arrangement and all source-slot limits are unchanged.

Implementation: firmware/src/reggae_seq_ui.c, expanded arranger test,
emulator version/cache key and user documentation. No DSP or file-format changes.
Rendering reuses the same scratch strip and 10 Hz budget.

Native screenshots: build/reggae/sequence-notes.png and sequence-drums.png.
Validation and hardware-build figures are recorded in TESTING.md.

Access: https://kemsthesage.github.io/reggae-os/?ui=0.4.1
Tap SEQ, choose a step; for drums choose a lane and turn HIT on/off.
Physical FM-1 and iPhone Safari remain untested.
