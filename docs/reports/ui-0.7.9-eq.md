# UI 0.7.9: four-band EQ and FX navigation

Each of the four tracks and master has four independent, cascaded parametric
bell bands (20 bands total), followed by the existing linked compressor.
Frequency is 40 Hz–16 kHz, gain ±12 dB, Q 0.5–4. Zero gain bypasses that band.
The processors remain in the shared native/WASM audio path.

One EQ page avoids four extra pages: K1 selects T1 B1–B4, then T2, T3, T4 and
M B1–B4. K2 selects frequency, K3 gain, K4 Q. The curve sketches the selected
band, not a measured or combined spectrum. All bands process simultaneously.

FX button and SELECT page navigation both follow:
PUNCH-IN → 4-BAND EQ → COMPRESSOR → COMP TIMING → FX SENDS → INSERT → INSERT 2 →
DELAY → REVERB → CHORUS. Page counters follow this order. Slicer/chopper and
pressure diagnostic pages are hidden, retaining page IDs and saved DSP settings
for compatibility. This is menu ordering, not a change to the audio signal chain.

## Storage

Old FUNC (0.7.8) projects import their bell into band 1, retain their compressor,
and initialise bands 2–4 to bypass. Unprocessed legacy projects still load.
FUNE adds the extra band values within the existing 3840-byte slot, using the
same bounded RLE payload and checksum validation. New processing projects need
0.7.9; earlier firmware does not understand FUNE. Oversized payloads fail before
replacing a slot. The larger settings header reduces compressed payload space
by 90 bytes; no flash address or partition is changed.

The 0.7.9 browser preview starts fresh with its own save key. Older preview
and stable saves are untouched. No hardware flashing was performed.

## Verification

Native ASAN checks cover all 20 bands' boost/cut response, combined response,
cascaded extreme gains and silence decay, linked compression, four-band recall,
old FUNC import, legacy format tests, FX order in both directions and hidden
pages. Native 240×240 captures verify band labels and page numbering.
WASM tests cover audible processing, unchanged dry baseline, independent saved
slots, repeatable fresh-instance recall, and the heavy song with all 20 bands
plus five compressors enabled. Host timings and build limits are in TESTING.md.
Physical latency, CPU headroom and listening quality remain hardware untested.
