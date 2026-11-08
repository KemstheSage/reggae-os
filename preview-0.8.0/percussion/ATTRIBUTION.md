# ROOTPERC optional percussion pack

Audio: CC0 1.0 Universal, Versilian Studios' [VCSL](https://github.com/sgossner/VCSL).
Retrieved from [SLOOP](https://github.com/isod89/sloop-fm1) commit
`d89364253b79a5c6002c0d90c5f16b3c3c16ea9d`, `assets/samples-cc0/KIT`.
See its separately declared [sample provenance](https://github.com/isod89/sloop-fm1/blob/main/assets/samples-cc0/ATTRIBUTION.txt).
No other SLOOP asset licence is inferred from this designation.

| Local file | SLOOP file | Original VCSL path |
|---|---|---|
| rim.wav | 12_rim_sidestick.wav | Membranophones/Struck Membranophones/Legacy Snares/OldSnare/snare_rim.wav |
| conga.wav | 02_Conga_HitN_v2_rr1_Sum.wav | Membranophones/Struck Membranophones/Conga/Conga_HitN_v2_rr1_Sum.wav |
| claves.wav | 03_Claves1_Hit_v2_rr1_Mid.wav | Idiophones/Struck Idiophones/Claves/Claves1_Hit_v2_rr1_Mid.wav |
| shaker.wav | 01_Mid_ShakerDouble_Down_rr1.wav | Idiophones/Struck Idiophones/Shaker, Small/Mid_ShakerDouble_Down_rr1.wav |
| tambourine.wav | 00_Tamb1_Hit_v2_rr1_Mid.wav | Idiophones/Struck Idiophones/Tambourine 1/Tamb1_Hit_v2_rr1_Mid.wav |

SLOOP trimmed/normalised the rim recording. REGGAE-OS keeps these source WAVs,
resamples to 22.05 kHz mono, normalises peak to 30000 and encodes IMA ADPCM for
one 80 KiB user slot. Five separate attacks, not loops from commercial records.
Map: F3/53 rim, G3/55 conga, A3/57 claves, B3/59 shaker, C4/60 tambourine.
Other notes are silent. Hold the note for the whole natural sample tail.

The emulator's optional load button installs into an empty slot, or reuses an
identical pack, and selects SAMPLE on the current track. Existing sample slots
are never overwritten. It changes that track's sound; use HOME/PRESETS to return
to the curated bank. Save a user preset/project through the usual controls.
This is a sample upload convenience, not a separate browser synthesis engine.

On future hardware, use the existing USB-MIDI uploader (choose an unused slot):

```sh
python3 tools/fm1_sample_upload.py load 1 ROOTPERC \
  assets/samples-cc0/REGGAE_PERC/rim.wav:53:53-53 \
  assets/samples-cc0/REGGAE_PERC/conga.wav:55:55-55 \
  assets/samples-cc0/REGGAE_PERC/claves.wav:57:57-57 \
  assets/samples-cc0/REGGAE_PERC/shaker.wav:59:59-59 \
  assets/samples-cc0/REGGAE_PERC/tambourine.wav:60:60-60
```

Then set the keyboard octave to 0, quantise OFF and transpose 0. Select
SAMPLE, SET USR1, attack 0, decay/sustain 127, release 30 and
turn down the level before balancing it. The hardware uploader replaces the
chosen slot: check `info` first. Physical upload and playback remain untested.
This optional pack uses melodic SAMPLE sequencing, not the DRUM eight-lane grid.
