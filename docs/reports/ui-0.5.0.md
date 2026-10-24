# UI 0.5.0 — first SLOOP-style recording integration

This is the first integration slice, not a wholesale firmware merger.

## Implemented

- Fresh start: T1 Roots Organ, T2 Deep Roots Bass, T3 Roots Trumpet, T4 Roots Kit.
- HOME: tap REC to arm. With internal clock, the first note starts the existing
  loop at the set tempo and records on step one. Physical keys and routed MIDI
  notes use the same entry path. PLAY can start an armed take without a note.
- REC during playback starts/stops an overdub on the selected track.
- Switching tracks ends the current overdub; press REC again to record the next
  instrument. STOP clears the live-recording arm. REC again cancels a waiting arm.
- Track numbers and recording/ready status on the native performance screen;
  blinking ready LED, red recording track, beat-responsive track indicators.
- The HOME curated-bank browser keeps T4 on Roots Kit. Advanced engine, full
  library and project loading remain compatible; this is not a global engine lock.
- Existing stopped STEP add/replace/clear gestures retained.

Sounds and synthesis settings are unchanged. No extra audio/event buffers and
no persistent format changes. Existing project loads retain their own sounds;
new startup defaults do not forcibly rewrite earlier projects.

## Deliberately not yet ported

Free-tempo first-take capture (infer tempo/length from performance), SLOOP's
16-lane drum storage, live section-change recording, and its recording settings
screen. Existing 8-lane kit/grid, pattern lengths, tempo, quantisation and song
arranger remain. This records notes, not OP-1-style audio tape.

External MIDI clock owns transport; a local note never starts an external-clock
session. PLAY with an armed internal-clock session retains existing count-in
settings. First-note start is immediate, without a count-in.

## Provenance and validation

SLOOP reference pinned and credited in CREDITS.md. Changes are in seq.c,
ui_input.c, ui.c, main.c and reggae_ui.c; same C code runs in WASM and firmware.
See TESTING.md for final test/build results. Hardware and iPhone Safari untested.
