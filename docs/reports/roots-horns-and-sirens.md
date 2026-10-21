# Roots horns and dub/jungle sirens — research direction

21 October 2026. Research and implementation constraints, not a new release.
Bank 0.4 remains deployed. Organ, piano, bass and approved graphics are unchanged.

## Listening references

These broaden the original Big Ship / Faded Away / Soldiers references. The goals
below are proposed comparison criteria, not claims of completed critical listening,
transcription, isolated-stem analysis or exact recreation of the recordings.

| Recording | Comparison goal | Source |
|---|---|---|
| Burning Spear — Marcus Garvey | Weight and balance of a roots horn section; compare a short phrase as well as a stab | [Recording context and horn arrangement](https://www.factmag.com/2016/01/06/burning-spear-beginners-guide/2/) |
| Tommy McCook & The Aggrovators — Roots of Africa | Acoustic horn body, articulation and register | [Label recording](https://pressuresounds.bandcamp.com/track/roots-of-africa) |
| Tommy McCook & The Aggrovators — The Bionic Horn | Contrasting horn phrasing within a roots arrangement | [Label album](https://pressuresounds.bandcamp.com/album/disco-rockers-super-star) |
| Aswad — Warrior Charge | A UK roots instrumental reference; judge the horn line in context | [Babylon distributor notes](https://kinolorberbucket.s3.amazonaws.com/production/documents/BABYLON%20press%20notes.pdf) |
| Jah Shaka — Verse 1 / Verse 10, Commandments of Dub | Dub space, entrances and effect tails; not a claim that every track contains the same siren | [Release and tracks](https://open.spotify.com/embed/album/0ExsIkFTlmJpUmx2REM5VU) |
| SHY FX — Gangsta Kid | Jungle siren vocabulary and placement | [Release history discussing its siren](https://www.boomtownfair.co.uk/news/how-shy-fx-changed-the-game-with-original-nutta) |
| UK Apachi & SHY FX — Original Nuttah | Ragga-jungle context and energy; distinguish effects from sampled vocals | [Artist recording](https://www.youtube.com/watch?v=_VFf6434lto) |
| DJ Gunshot — Wheel Up | Candidate for short jungle signal articulation; confirm the original version during listening, not a later remix | [Artist catalogue](https://music.apple.com/us/artist/dj-gunshot/290110929) |

Bobby Ellis describes the replacement of multi-player horns with synthesizers in
his [interview](https://www.reggae-vibes.com/articles/interview/2017/11/bobby-ellis-interview/).
This is useful first-person context for why an oscillator brass patch is not the
same target as a roots horn section. Saxophone belongs in this comparison even
though it is technically a woodwind.

## Sample feasibility: confirmed architecture, estimated fit

The shared firmware SAMPLE engine already decodes IMA ADPCM and supports pitched
sample zones, envelopes and filtering. This is a hardware route, not browser-only
audio. Versilian Studios explicitly publishes VSCO 2 CE under CC0:
https://versilian-studios.com/vsco-community/
https://github.com/sgossner/VSCO-2-CE/blob/master/LICENSE

The existing tools/fetch_cc0.py even defines a BRASS selection from its trumpet
sustains, although those samples are not in our active built-in bank. Existing
VCSL tenor sax recordings are already present. A compact trumpet prototype is the
next candidate, followed by a trumpet/sax blend if the sound and budget justify it.
Orchestral source recordings still need suitable articulation and voicing; a sample
does not automatically make a patch stylistically convincing.

At the current 22,050 Hz, mono, 4-bit storage rate, three 0.95-second zones need
approximately 31,422 bytes of encoded audio, plus zone metadata/alignment. The
bank-0.4 application was 521,344 bytes; packaging allows 0x8DFBC = 581,564 bytes,
leaving 60,220 bytes before new code/data. This suggests one small set can fit;
it does not prove that a finished multi-instrument section fits. Two such sets
already exceed that remaining space before overhead. Confirm with an actual
hardware build. Streaming decode avoids a large uncompressed sample buffer.

Compatibility work is necessary: existing sample IDs 0–4 and user IDs 5–7 are
persistent. Simply appending a built-in set shifts user IDs. SAMPLE, GRAIN,
waveform display and enum canonicalization currently assume contiguous built-ins.
Preserve IDs explicitly and test old user slots before integrating new data.
Do not reuse a retired alias casually or alter the approved piano sample bytes.

Proposed horn audition: retained natural tongued onset, short and sustained notes,
modest velocity-to-level/tone response, short clean release and restrained room.
Avoid fixed major/minor chord samples so the musician can play their own harmony.
Compare dry first, then with effects. Evaluate over a realistic horn register;
large transpositions of a single sample are not a substitute for multisampling.
No samples from the reference commercial recordings are to be redistributed.

## Siren design candidates

Use original synthesis, keeping real-time pitch/rate/filter/echo controls.
[Benidub DS01's manufacturer manual](https://benidub.com/wp-content/uploads/2022/03/Benidub-DS01-User-manual.pdf)
shows oscillator, LFO, manual modulation, trigger and VCA with external effects.
That supports independent source articulation and echo-tail design.

1. **Dub Wail:** sustained rounded tone, slow-to-medium continuous pitch movement,
   live sweeps and separate echo tails. Evaluate both sine and filtered pulse.
2. **Jungle Signal:** retriggerable short falling pitch envelope with a distinct
   end, adjustable fall speed/range, fast cutoff and short echo throws. This is a
   proposed original sound, not a claimed reconstruction of a particular sample.
3. **Fast Alarm:** optional stepped/two-pitch or rapid modulation variant, provided
   the first two remain immediate and useful. Do not turn every patch into a long
   repeating wail.

## Acceptance gates before release

- Audition actual shared-firmware renders, level-matched, dry and with effects.
- Horn single notes, repeated stabs and two/three-note voicings; note-off and loops.
- Siren one-shot retrigger, held notes, modulation extremes and effect tails.
- No silence, clipping, unstable values or stuck notes; native/WASM PCM agreement.
- Forty protected organ/piano/bass renders retain their existing reference hashes.
- Old sample/user-slot IDs, saved patches and piano bytes retain compatibility.
- Hardware package fits; physical FM-1 performance remains untested.

No patch changes or new audio auditions were produced by this research pass.
