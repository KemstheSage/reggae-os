# UI 0.7.4 — musician feedback

- SAVE enters PROJECT, ready to save a complete beat. USER is labelled SOUNDS;
  loading a sound explicitly reports that the sequence is unchanged.
- PUNCH-IN is a persistent FX page using the existing performance layer. Tap FX
  through it or hold/release FX to enter; tap FX or another page to leave.
- Preset lists on all pages are restricted to the approved bank plus user sounds.
  Engine shortcuts load approved patches or refuse unsupported engines. Older
  projects remain loadable; HOME browsing recovers from a stock sound.
- Four tracks retained. Eight tracks requires a separately scoped storage/DSP/UI
  redesign and hardware profiling, not just changing a constant.

Save diagnosis: regression saves different drum hits, chord notes and individual
velocities to A and B, verifies A's bytes did not change when B was saved, clears
RAM caches and reloads both from emulated NOR repeatedly. Both remain distinct.
The sound-slot path reproduces unchanged current beats because it restores only
the instrument. The user believes they used PROJECT, so the reported hardware problem remains
unreproduced. A full physical-input PROJECT test also saves distinct beats
through the naming dialog to C/D and recalls both correctly. Slot selection alone
is not a load. Occupied-slot selection now arms LOAD, not a stale SAVE; saved and
loaded messages identify the slot. Failed project saves leave the name dialog open.

Tests and build results are recorded below after verification. No physical flash
or physical power-cycle verification is performed for this update.

- Final native feedback test passes, including occupied-slot overwrite, invalid
  format refusal without closing NAME, and drum-track recovery from a stock patch.
- Address/undefined-behaviour sanitizer feedback test passes on the core save,
  navigation and punch-in changes.
- WASM project persistence test passes: A/B have distinct PCM hashes, A reloads
  identically in separate fresh instances. A's flash bytes survive saving B.
- Final WASM note velocities, AUTO voice leading, step gestures and baseline/
  heavy-song emulator checks pass. Sound-slot persistence expectations were
  updated to explicitly navigate to SOUNDS; FX release now expects a persistent page.
- Guide regenerated with 103 native framebuffer references; PUNCH-IN inspected
  at 240×240.
- Hardware build passes linker/register/RAM checks: app 545,512 bytes;
  static RAM 61,032 / 73,728, pool 328,896 / 368,640. Package 610,090 bytes.
- Candidate: `build/feedback-0.7.4/reggae-os-ui-0.7.4.fwsc`.
  COMPILES / EMULATOR TESTED / HARDWARE UNTESTED. Not flashed.

Final firmware SHA-256: `33a8917d6cd0a48f89e488cf6bdd5ae38b92586db4a5e9508c784eb44015a4f8`. Package passed the sanitized OTA simulator.

REGGAE regression coverage completed: 75 preset/control audio renders, 40 protected
sound hashes unchanged, graphics and siren, arrangement/recording/step editing,
sequencer menus, mixer audio, 546 screen renders, settings/naming, extended chords,
AUTO, individual velocities and feedback tests. The run was interrupted after the
extended-chord check; the remaining three native tests were rebuilt and run with
-O0 on resume. Final WASM project/audio/navigation checks all pass.
