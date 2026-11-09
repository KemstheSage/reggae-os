# Riddim OS 0.8.2: acoustic drum audition candidate

Four finished audition presets, pending musician feedback and physical testing:

| Preset | Sound design |
|---|---|
| Roots Kit | Eight acoustic one-shots: kick, snare, clap, closed hat, open hat, tom, cross-stick, cowbell. Restrained cymbal balance; no sample loops. |
| Dusty HipHop | Same source kit, down-tuned, shorter decay, stronger initial transient, darker filtering and compensated saturation. |
| Soft LoFi | Same source kit, further down-tuned, softer transient, darker filtering and gentler saturation. |
| Ambient Perc | Synthesized low drum, conga and bell; acoustic snare/clap/hats/rim with softened attacks and reverb. |

These are three treatments of one acoustic recording set plus a hybrid kit, not
four independently recorded drum kits. The source kick is an orchestral bass-drum
recording, shaped here into a short low drum; it is not a sampled reggae drum set.
The objective is a useful audition candidate, not a claim of acoustic authenticity
established by numerical tests. Original CC0 recordings are credited in
`assets/samples-cc0/ROOTS_DRUM/ATTRIBUTION.md`.

## Try it

Open `/preview-0.8.2/`, tap **Switch on**, then **Load new acoustic drum kits** below
the instrument. This installs RDMDRM2 into an empty emulator sample slot and selects
Roots Kit. Turn PRESETS through Roots, Dusty HipHop, Soft LoFi and Ambient Perc.
The four performance knobs are **Tone, Decay, Snap, Drive**. EDIT retains tuning
and lane level controls. Closed hats choke open hats; note-off does not truncate
one-shot drums. Play C2 kick, C#2 rim, D2 snare, D#2 clap, F#2 closed hat, A#2 open
hat, A2 tom and G#3 cowbell (MIDI 36/37/38/39/42/46/45/56).

The preview has its own browser-save namespace. Existing previews and physical
beats are untouched. The pack persists with the emulator's flash. No empty slot:
the upload is refused and existing samples remain intact. Without RDMDRM2 the
new presets fall back to synthesized drums; load the pack to hear these auditions.

The audition page includes identical original 80 BPM test grooves for a controlled
comparison, plus tours of all eight hits. These are tests of timbre, not recordings
of commercial songs or claims to reproduce a particular drummer's performance.

## Compatibility

- Existing stored kit IDs 0–11, preset IDs and project format remain unchanged.
- New kit IDs 12–15 and three appended library entries; 31 sounds total.
- Loading the factory Roots preset gets the new model. Existing saved beats retain
  their previous drum parameters and will not automatically switch to sampled hats.
  To change an old beat, load the new kit on its drum track, then save a new project
  slot after auditioning. Keep the original project and backup.
- Standard 80 KiB user sample slot; pack uses 33258 bytes. No partition changes.
- Application image: 580000 bytes. Static RAM: 61960 / 73728 bytes;
  allocated pool: 338124 / 368640 bytes. Application XIP is nearly full. Embedded PCM was rejected by a real hardware
  linker check; samples therefore live in the existing backed-up sample area.
- The C renderer is shared between WebAssembly and FM-1. Hardware build passes;
  new kit CPU cost, audio quality and behaviour on the physical unit are untested.
- No hardware update or sample upload has been performed for this candidate.

## Validation

ASan native tests: non-silent grooves, finite/bounded output, hat choke, retrigger,
velocity response, natural ends and no remaining active voices. WASM tests: all
eight lanes of all eight library kits, simultaneous hits, rapid hats, finite
output, tails to silence and distinct kit fingerprints. Worklet upload tests:
CRC rejection, all-slots-full refusal, no changes outside the chosen empty slot,
and repeat upload reuses an identical pack. Project A/B save/recall and complete
backup tests pass. Forty protected organ/piano/bass renders remain byte-identical.

Reproduce pack: `python3 tools/reggae/gen-roots-drums.py`.
Run WASM checks: `node tests/riddim_drums_wasm.mjs` and
`node tests/drum_pack_worklet.mjs` after building the emulator.
