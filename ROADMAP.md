# Milestones

1. Foundation: upstream v1.6.1 and pinned engines cloned; architecture investigated;
   baseline hardware/emulator builds complete. Full test runs completed with documented
   baseline failures; see TESTING.md.
2. Identity: implemented first hardware-native organ/bass performance screen,
   branding, four parameter columns and bounded animation. Broader screen polish remains.
3. Organ/bass: implemented four new synthesis patches and direct knob assignments;
   audible-output, release, persistence and native/WASM parity checks passed.
   Desktop emulator verified. Roots Organ 0.2 audition approved as the starting point
   on 15 October 2026; preserve it and develop separate organ variants. Other
   presets still need musician approval; physical testing remains.
4. Extended instruments: bank 0.3 adds melodica, electric piano, piano chop, brass,
   skank, siren, FM bass, dub organ and an eight-piece kit. All require musician
   audition/refinement; acoustic realism is limited.
5. Dub effects: retained upstream effects; FX, delay and reverb now have native
   animated illustrations. Siren/dub organ expose delay send. Echo throws and
   deeper tape/spring refinements remain.
6. Presets: retained flash storage, full browsing and save/recall. Fifteen-sound HOME bank
   implemented; illustrated genre/category browsing remains.
7. Hardware optimisation: no physical device. Linker and target builds are necessary
   but cannot establish DAC quality, timing, controls, flashing or recovery on hardware.
8. Final polish: pending auditions and hardware validation.

This repository is an early instrument prototype, not the complete final vision.

User direction, 16 October 2026: ultimately remove all Felucca code. This is a
separate later rewrite, not satisfied by renaming the fork or hiding attribution.
Current work continues in the existing prototype to develop and validate sounds,
original UI design and interaction requirements. Research and first native UI
refinements are recorded in `docs/research/te-interface-study.md`.

Bank 0.5 adds a compact sampled roots trumpet and separate dub wail, jungle signal
and fast alarm candidates. Horn section layering remains future work. See
[bank 0.5](docs/reports/bank-0.5.md) for validation and limitations.

UI 0.4.0 begins the expanded workstation request: native note/drum editing and
song arrangement now share the REGGAE-OS layout. Existing four-part, four-source,
sixteen-section song playback is retained. Next: mixer, automation and project
management visual/workflow pass; full device timing and usability validation.

UI 0.4.1 simplifies the main pattern editor into sixteen large pads and a single
drum lane. Deep editing remains available. The next usability work remains
project saving/browsing, mixer and automation, with musician feedback on the pads.

UI 0.4.2 restores the previous piano-roll/drum-grid view at the musician's request
and introduces explicit add/replace/clear gestures for stopped note-step editing.

- UI 0.5.0: first SLOOP-style integration — HOME arm/first-note start, selected-track
  overdub, safe track handoff, T4 Roots Kit startup and performance-browser role,
  native recording/track indicators. Next: free-tempo take capture and live
  section-change recording; neither is implemented in this slice.

- UI 0.5.1: SEQ visual refinement and selected-track whole-piano-roll clear with
  a one-second EDIT+SEQ hold and SAVE undo. Further SLOOP integration is deferred
  while the current editing workflow is refined.
- UI 0.5.2: animated record character on SEQ/arrangement, driven by actual voice
  activity and sequencer position; clear-roll shortcut retained.

- UI 0.5.3: tabletop machine styling for the roll, drum grid and arranger; native
  pixel glyphs, responsive rotary indicators and existing character animation.

- UI 0.5.4: correct character/header spacing and keep help text inside the case;
  fit long labels within their allocated panels.

- UI 0.5.5: smaller performance-screen instrument/category names.

- UI 0.5.6: smaller sequencer headings and clear separation from tempo/status.

- UI 0.5.7: compact font for all four sequencer parameter labels and values.

- UI 0.5.8: compact shortcut hints; unified detail, automation, pattern tools,
  Euclid preview and cleanup screens. Project/mixer screens remain future work.

- UI 0.5.9: remove the sequencer hint background strip.

### UI 0.6.0 — live performance request

Implemented: four simultaneous level controls with mute/solo speaker characters;
sixteen punch-in pads including new SLOOP-derived gate and mono; direct
root/scale/chord/voicing controls with singing chord notes; optional credited
CC0 percussion pack through existing sample slots and the actual firmware
sampler. Jiant/Orchid/HiChord research and scope are documented. Not a wholesale
SLOOP port, not Orchid's proprietary engine, and not a new per-lane sampled
DRUM engine. Hardware and iPhone verification remain outstanding.
