# Live performance research and implementation — UI 0.6.0

## Sources inspected

- [SLOOP](https://github.com/isod89/sloop-fm1), commit `d89364253b79a5c6002c0d90c5f16b3c3c16ea9d`: `firmware/src/punch.c`, `drum_synth.c`, `tools/gen_drumkits.py`, `LICENSING.md`, separately credited sample sources.
- [JIANT](https://juanjiant-bit.github.io/JIANT/), source commit `302377c86a3942fa06bed588d68d06507e735201`: `firmware/src/ui_engvis.c` and actual screen atlas `docs/jiant-screens.png`.
- [Orchid playing guide](https://telepathicinstruments.com/pages/how-to-orchid), [getting started](https://help.telepathicinstruments.com/getting-started-with-orchid-1324868).
- [HiChord manual](https://updater.hichord.shop/manual/).

These references informed the work; this is not a claim of equivalence to those instruments.

## Punch-in effects

SLOOP has 27 effects and a 65,536-byte mono audio ring. Copying that complete
subsystem would exceed our remaining pool margin. REGGAE-OS already has stereo
repeat, reverse, filters, tape stop, freeze, pitch shifts, flanger, phaser and
reverb/echo throws. Its existing buffer is retained.

Adapted SLOOP's PX_GATE idea/64-sample edge ramps to our 1/16 clock and stereo
mix path. Added mono averaging with a 64-sample wet ramp. No new audio ring.
GATE and MONO fill the last two unused white keys. Existing effects' numeric
IDs and explicit NONE mappings remain stable; zero/default maps gain the two
new defaults. SLOOP's entire 27-effect bank has **not** been ported.

Hold FX: sixteen labelled white-key pads; first four black keys momentarily
mute tracks. Knobs remain filter, crush, throw and depth. Effects are master
bus effects, not per-drum inserts. Long repeats still depend on available
buffer length at the chosen tempo. The new screen dims unavailable effects.

## Drums and reusable audio

SLOOP's synthesized kits combine pitched bodies, noise, metallic sources,
clicks, filters and clap envelopes. K808/K909 and JUNGLE provide electronic
rather than acoustic roots-reggae references; DEMBOW is a reggaeton kit, not a
roots kit. A whole engine transplant would duplicate our existing drum voices.

Selected five individually attributed VCSL CC0 recordings from its KIT folder:
side-stick, conga, claves, shaker and tambourine. These complement, rather than
replace, ROOTS KIT. `tools/reggae/percussion-pack.py` builds the existing user
slot format: 44,637 bytes ADPCM, five zones, below the 81,408-byte data limit.
The emulator upload convenience uses the same NOR slot and SAMPLE renderer
as hardware. It only installs into erased slots, reuses an identical pack,
and selects the sound on the current track. No existing sample is overwritten.

This pack is pitched SAMPLE playback with exact-key zones, not a new DRUM kit;
it uses the melodic piano roll and needs held notes for natural tails. It can
be saved in projects/presets with the normal storage workflow. Hardware USB
upload instructions and full provenance accompany the source WAVs.

The first WAV tour is the resampled sources; `firmware-tour.wav` is the actual
WASM firmware decoding them. Numerical evidence verifies audible output,
bounded peaks and silent releases. This does not establish subjective musical
quality or acoustic realism. No copyrighted record was sampled.

## Chord mode

Orchid makes chord types/extensions and voicing directly playable. HiChord
centres diatonic chord buttons and visible notes, with a joystick for changes.
We reuse the firmware's tested chord engine: one key produces a triad or seventh,
with fixed major/minor/seventh/suspended/power options and close/open/inverted
voicings. SCL twice opens CHORD GARDEN. Four knobs now expose root, scale,
chord mode and voicing together; the last played chord's notes have original
singing faces. Set K2 to MAJ/MIN for an explicit diatonic context.

This does not implement Orchid's full voicing engine, joystick input, custom
extensions, automatic voice leading or strumming. Shared polyphony is eight
voices; one seventh uses four. Monophonic modes play the root; drum tracks do
not expand chords. Keyboard, MIDI, arp, recording and release behavior remain
covered by the existing chord tests.

## Characters and four-track mix

Jiant's useful principle is a distinct visual species per synthesis mechanism,
with actual audio activity and parameter state informing movement. We inspected
its operator creatures, drawbar forms and per-hit percussion identities. No
Jiant artwork or source was copied.

Our original mixer has four speaker companions. A muted/unsoloed channel closes
its eyes; active voices open the mouth; its fader shows actual track level.
This is voice activity, **not an audio peak meter**. Chord notes form a singing
garden. Existing REGGAE-OS instrument characters remain intact. Both run in the
shared 240×240 firmware renderer at 10 Hz, outside the audio callback.

Hold GLO for four simultaneous level knobs. First four black keys toggle mute;
F3/G3/A3/B3 solo tracks while held. Double-tap GLO locks the layer; tap GLO to
leave it. Existing selected-track mixer/pan/send pages remain available.

## Verification limits

FM-1 packaging and memory guards pass; no physical FM-1, iPhone or Safari-device
test has been performed. Native panel tests and WASM sample output are exercised.
The upstream Mac instruction-counter test fails its harmonizer and modulation
cost thresholds on both the preceding release and this version. Functional
checks pass; these host estimates do not certify real-device CPU headroom.
