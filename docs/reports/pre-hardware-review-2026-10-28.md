# Pre-hardware review — 28 October 2026

Research and recommendations only. No firmware changes, device probing or flash
operations performed during this review. REGGAE-OS remains UI 0.7.1 on Felucca
1.6.1; physical operation is still untested.

## Current upstream references

- [Felucca 1.7.0](https://github.com/hugelton/Felucca/releases/tag/v1.7.0),
  released 28 October: external-clock drift, TRS receive/timestamp fixes, signed
  DSP/FM6 arithmetic fixes, transient-zero-BPM protection and stock return changes.
  Hardware now lives in a separate BSP submodule; dual core remains beta/off.
  Review individual fixes against our pinned base before a wholesale rebase.
- [SLOOP](https://github.com/isod89/sloop-fm1): README reports 2.8.3; useful
  references are held-note persistence during recording/playback, busy-pattern
  kick reliability, sample retrigger attacks and adjustable punch-in fades.
  These are upstream claims, not independently reproduced on hardware here.
- [ChoralRoot](https://github.com/Quixotic7/ChoralRootFM1/blob/main/docs/MANUAL.md):
  manual currently describes 0.27 (README has older release summaries). Voicing
  knob moves the lowest note up or highest down an octave; performance modes
  and per-step voicing are useful musical references. Do not copy its hardware
  layout wholesale: it is a dedicated chord instrument.
- [JIANT](https://github.com/juanjiant-bit/JIANT): public installer has multiple
  distinct SAMPLE/ADDITIVE/STANDARD revisions. Do not equate all README development
  features with the stable package. Useful ideas: control acceleration, mix
  continuity, visible performance feedback. Different engine-slot meanings make
  project interchange unsuitable without explicit conversion.
- [X0X](https://github.com/charlesvestal/fm1-x0x): 808/909, two 303s, rearranged
  breakbeat playback and recorded knob moves. Percussion articulation and groove
  interaction are relevant; a wholesale sound-engine import is not needed.
- [Clipper](https://github.com/isod89/clipper-fm1): README reports experimental
  1.1, extensive synth engines/macros and level-matched sounds, but little real
  FM-1 use. Useful later sound-design reference; not evidence that more engines
  or CPU work is safe in our four-track build.

## Proposed order

1. Review/backport applicable clock, MIDI, numerical and zero-BPM fixes, with
   focused regression tests; inspect exact stock-return/update compatibility.
   Preserve the known 0.7.1 package and its checksum as a rollback candidate.
2. Extend voicings, keeping existing IDs/meaning: third inversion for four-note
   chords; drop-2; rootless accompaniment; optional compact three-note shells.
   Define fallback behaviour for triads and keyboard boundaries explicitly.
   Rootless parts leave the bass track room and reduce shared-voice pressure.
3. Later add bounded automatic voice leading (nearest suitable inversion), with
   deterministic resets on transport/project changes and robust note-off tracking.
4. Review held-note, retrigger and eight-voice pressure with drums/bass/chords
   together; protect kick/bass continuity where measured problems justify changes.
5. Smooth punch-in engagement/disengagement, without changing approved sound
   presets. Add a small set of original one-drop/steppers/rockers starting patterns
   only as optional templates, never replacing users' patterns.

These are proposed additions, not implemented features. More chord types were
already added in 0.7.1; more voicings changes placement of those notes, not harmony.

## First physical session

Before changing stock firmware, record its version and verify keys, encoders,
buttons, charging, speaker, headphone output and USB enumeration. Keep the correct
vendor recovery package and instructions available. Do not trial random firmware
switches: ChoralRoot explicitly documents incompatible installation paths.
Use a computer/data USB connection for installation; the iPhone emulator link is
not an installer. Inspect connected identity before choosing the local REGGAE-OS
package; never bypass model/identity checks with --force.

After installation, verify actual control mapping and output levels first, then
save/reboot/recall, note releases, four-track playback under animated UI and effects,
and external MIDI clock. Listen through headphones/line out as well as the built-in
speaker. Measure on-device CPU/underruns before enabling beta dual-core options or
adding heavier DSP. A successful emulator or linker check cannot establish these.

Current build evidence: application 542,596 / 581,564 bytes (38,968 bytes free),
static RAM 61,000 / 73,728, pool 328,896 / 368,640. Flash space is not a measure of
real-time processing headroom. No physical test result is implied.
