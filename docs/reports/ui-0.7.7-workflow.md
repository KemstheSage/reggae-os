# UI 0.7.7 DEV: workflow foundation

This is a development slice, not the completed EQ/compressor and note-editor
request. It has not been installed on the FM-1 or published to GitHub Pages.
The installed firmware and saved hardware projects were not touched.

## Implemented

- FX enters persistent PUNCH-IN first; another tap opens FX SENDS.
- FX SENDS retains the animated drive, chorus, echo and reverb artwork. It shows
  which track is selected, and distinguishes direct drive from sends.
- SLICER moves out of FX to EDIT as CHOPPER. Its existing rhythmic gating DSP,
  parameters and page ID are unchanged.
- Drum-grid K4 edits the selected hit's persistent velocity, without changing
  simultaneous hits in other lanes. K1 selects step, K2 drum, K3 adds/removes a
  hit. Empty cells must be given a hit first. Existing accent gestures remain.
- The existing 128-entry automation/velocity storage limit still applies; a full
  pool reports an error. No storage format or flash layout changed.

Source: ui.c, ui_input.c, params.c, ui_draw.c, ui_help.c, reggae_seq_ui.c,
reggae_ui.c and reggae_workspace.c. Tests and SEQUENCER.md document the workflow.

## Verification

Native AddressSanitizer workflow test passes: actual panel navigation, animated
FX screen dispatch, delay-send editing, independent kick/hat velocities and
project save/recall. Separate feedback, per-note velocity and pressure-input
regressions passed. Screenshots are rendered at the physical 240 × 240 resolution:
`build/reggae/workflow-grid.png` and `build/reggae/workflow-sends.png`.

Final firmware and WASM builds passed. Final real-WASM tests passed for audio,
MIDI controls, deterministic playback and saving distinct project slots into NOR
then recalling them in fresh emulator instances. Host timing is not an FM-1 CPU
measurement. Final build sizes and checksums are recorded in TESTING.md.

## Still required

- Parametric EQ and compressor on every track and on the master, with bypass,
  persistence and audible DSP validation. Existing drive, filters and limiter do
  not satisfy this request.
- Individual note lengths and repeats, including a hi-hat rolling independently
  of a simultaneous kick; reliable note-off handling across steps and sections.
- Melodic velocity editing directly in the roll and consolidation of detail pages.
- A tested versioned persistence extension: the existing 3840-byte project payload
  cannot simply be enlarged, and spare bytes are insufficient. Old beats must
  load intact and interrupted or capacity-exceeding saves must preserve the last
  good slot. No sample storage may be silently overwritten to make room.
- Physical FM-1 and iPhone interaction testing of this development version.

The public emulator and screenshot guide still describe the published version;
they do not yet expose these development changes. Build locally with BUILDING.md.
