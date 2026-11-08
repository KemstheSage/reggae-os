# UI 0.7.8 preview: processing and individual note editing

Shared firmware implementation for FM-1 and WebAssembly. No physical installation
or hardware listening test was performed for this release. Do not overwrite an
existing beat to test the new format: retain an older slot or backup.

## Controls

From HOME, FX enters PUNCH-IN, then FX SENDS. Continue to BELL EQ, COMPRESSOR
and COMP TIMING. Each new page uses K1 to choose T1–T4 or MSTR (master).

| Page | K2 | K3 | K4 |
| --- | --- | --- | --- |
| BELL EQ | Centre frequency, 40 Hz–16 kHz | Gain, −12…+12 dB | Q, 0.5…4 |
| COMPRESSOR | Threshold in dB below full scale, 0…48 | OFF, 2:1, 3:1, 4:1, 8:1, 20:1 | Makeup gain, 0…12 dB |
| COMP TIMING | Attack, 1…100 ms; 0 = default 10 ms | Release, 1…1000 ms; 0 = default 150 ms | Same ratio/bypass |

This is **one fully parametric bell band per target**, not a multiband equalizer.
Zero EQ gain bypasses the EQ; compressor ratio OFF bypasses compression and makeup.
Start with threshold 18–24, ratio 2:1 or 4:1, and no makeup. The meter shows actual
linked peak-detector gain reduction; the EQ graph is a parameter sketch, not an FFT.

Track processors follow distortion/insert and precede track level, pan and FX
sends; spread receives processing on both channels with a linked compressor.
Master processing includes the shared effect returns and precedes punch-in and
existing output protection. The existing limiter is retained separately.

## One note editor

On SEQ's main roll/grid:

- K1: step.
- K2: individual note within the chord, or drum lane.
- K3: VELOCITY, LENGTH, REPEAT, or PITCH (HIT for drums).
- K4: edit the selected field for that note/hit only.

Use HOLD EDIT + keys to add notes and HOLD REC + keys to replace a step, as
before. Drum white keys still toggle hits; HIT with K4 also adds/removes one.
Native note/hit colours are green (1–42), yellow (43–84), red (85–127),
using the played velocity including inherited accents. Selection uses a cream
outline; muted drum lanes remain dim.
Velocity is 1–127; turning below 1 restores the original step velocity/accent.
LENGTH is in **steps**, not seconds: quarter-step increments through 4, whole
steps through 12, then 16/20/24/32/40/48/64. STEP inherits the original timing.
REPEAT is STEP (inherit) or x1/x2/x3/x4/x6/x8/x16, evenly spaced within the step.
Explicit drum lengths choke synthesized drum tails with the existing short fade.
An explicit length is the gate for each repeat; a repeat retriggers its own pitch.

For four hats over one kick: select the hat lane, K3 REPEAT, K4 x4. Keep the
kick on STEP/x1. Select LENGTH to shorten that hat independently. Notes already
sounding keep their scheduled duration until retriggered. Explicit per-note
scheduling takes precedence over legacy slide behavior on the edited step.
Timing uses the existing audio control-block resolution; no browser timers.

The separate velocity page is hidden from navigation, retaining its saved page
ID. DETAIL remains for step probability, global ratchet, nudge and velocity.
Pitch selection and the new length/repeat values are reflected in the native grid.
High repeat counts use a compact split marker plus the exact xN readout.

## Persistence and bounds

Existing projects load with processors bypassed and note timing inherited.
Unmodified processing/timing still writes the previous compatible format. New
processor settings or per-note timing use FUNC: a fixed 3840-byte envelope with
bounded RLE of the legacy record, processor state and independent checksums.
Timing uses FUND inside that envelope. Older firmware rejects the new format;
there is no promise that it can load a newly saved 0.7.8 project.

No flash addresses were changed. All writes continue through the existing
alternating-sector, checksum-verified, header-last storage system. An oversized
or invalid record is refused **before** replacing a slot. Incompressible dense
projects can hit this limit: SAVE FULL / INVALID DATA means keep the existing
slot and back up/edit the current work; it does not mean the save succeeded.

Note timing and velocity share the existing 128 sparse entries with locks and
automation (one timing entry and one velocity entry per edited note). Full pools
refuse additional entries. They use the existing undo, copy and note-removal
operations. Runtime scheduling is bounded to 16 pending events per track, with
existing shared voice allocation still governing audible polyphony. Note
ownership prevents stale timers releasing a newly played note of the same pitch.
Stop and arrangement section transitions clear pending repeats.

## Validation

- Native ASAN: five processor targets, approximately ±12 dB EQ response at centre,
  compressor 4:1 transfer, stereo linking, corner frequencies/Q, decay to silence,
  exact bypass, legacy/new save recall, invalid-data rejection and panel controls.
- Native ASAN: independent note lengths across steps, old-timer/new-note ownership,
  four hat triggers against one kick, stop, note removal and persistence.
- Historical project conversion suite and flash torn-write/oversize suite pass.
- Shared WASM: panel-created dry/processed/rolled beats remain audible and distinct
  after fresh-instance flash recall; previous project slot stays unchanged.
- Hardware compile/memory and final host timing results are recorded in TESTING.md.

No physical CPU, latency, iPhone Safari stress test or full upstream suite pass is
claimed. All processing uses bounded fixed-point DSP; no browser-specific synth.
The preview has a separate browser save key and starts fresh. Stable and 0.7.7
emulator saves are left intact. Source and third-party licences accompany it.


EQ coefficient reference: [W3C Audio EQ Cookbook](https://www.w3.org/TR/audio-eq-cookbook/),
peaking-EQ equations. The implementation uses fixed-point coefficients and internal
extra precision to avoid audible low-frequency limit cycles. Bounded coefficient
long division avoids a missing signed 64-bit divide library on the FM-1 toolchain.
A final internal headroom guard protects the inherited fixed-point mixer from
integer overflow when boost and makeup are combined at extremes.
