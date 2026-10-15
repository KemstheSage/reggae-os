# Sound design — revision 0.2

The first bank was functional but the musician's audition rejected its stylistic
character. Passing DSP tests is not evidence of convincing reggae sound. This
revision corrects identifiable synthesis problems; it is a new audition candidate,
not a claim to have matched a commercial recording.

## Approved organ starting point — 15 October 2026

The user auditioned Roots Organ from sound bank 0.2, liked it, and explicitly
approved it as the starting point. Preserve this sound while developing additional
organ registrations as separate variants. Compare changes to shared DSP against
this reference so refinements do not accidentally lose its character.

The exact settings and WAV checksum are in
[roots-organ-approved.json](assets/reggae-reference/roots-organ-approved.json).
The original firmware-rendered audition is preserved as
[roots-organ-approved.wav](assets/reggae-reference/roots-organ-approved.wav).
Its original composition/rendering code is tests/reggae_demo.c at commit ae87707;
no commercial recording is included. Approval applies to this organ audition only.
Other sounds and direct similarity to the reference records still require audition.

## Listening brief

The user's references are Freddie McGregor **Big Ship**, Kabaka Pyramid and Buju
Banton **Faded Away**, and Steel Pulse **Soldiers**. Treat the released studio
versions as the brief unless the user specifies another version. Keep separate
listening criteria for bass attack/body/decay, keyboard instrument identity,
voicing/register, articulation, and mix effects. Do not mistake a piano/guitar
chop for an organ, or assume every part uses FM.

The [Gleaner's interview with Freddie McGregor](https://past.jamaica-gleaner.com/article/entertainment/20170507/story-song-freddie-mcgregor-sees-big-ship)
identifies the Big Ship session's piano player as Wycliffe 'Steelie' Johnson and
bassist as Errol 'Flabba' Holt, with Roots Radics at Channel One. This is a concrete
reason to prioritise piano chops and bass-guitar-like articulation alongside organ,
rather than label a generic organ patch 'Big Ship'. The [official Faded Away release](https://www.youtube.com/watch?v=jyJ2pjS9hfw)
credits production to Damian Marley. In his [first-person interview](https://discover.ticketmaster.co.uk/interview/kabaka-pyramid-56983/),
Kabaka explains the use of Junior Byles' Fade Away in the newer recording. That
links the modern reference to an older recording; it does not identify its synth
patch, and we do not redistribute that sampled audio. [Steel Pulse's label](https://www.rhino.com/aod/true-democracy-steel-pulse)
places True Democracy in 1982. These credits establish context, not exact patches,
drawbar settings, instruments on each track, or spectrum measurements.

No direct audio comparison or listening to these recordings has been verified in
this session. No isolated stems, exact registrations or instrument settings have
been obtained. The candidate changes below are engineering inferences to audition
against these references, not transcriptions or artist-endorsed recreations.

## Instrument evidence and synthesis choices

[Aston Barrett's first-person interview](https://www.notreble.com/buzz/2014/06/26/legend-an-interview-with-aston-family-man-barrett/)
describes his flatwound strings. It supports researching electric-bass articulation,
not assuming that every roots bass is a sustained sine or that Barrett's equipment
was used on the user's chosen records. Our subtractive approximation keeps low
harmonics, gives the attack a small brightness transient, and lets amplitude decay.
It still lacks finger/string interaction and is not an acoustic model.

The [Hammond reference guide](https://hammondorganco.com/wp-content/uploads/2024/11/M-solo-Reference-Guide-Version-1.2.pdf)
documents drawbars and percussion. Local WHEEL source confirms nine partials and
single-trigger percussion. Registration positions are 16′, 5⅓′, 8′, 4′, 2⅔′, 2′,
1⅗′, 1⅓′, 1′. Neither new registration is claimed as a verified artist setting.
Organ articulation comes from the player's note lengths and alternating voicings;
it cannot be baked into a stationary tone. No automatic bubble is added to notes.

[Yamaha's FM tutorial](https://yamahasynth.com/learn/synth-programming/fm-101-part-six-complex-systems/)
explains that carrier amplitude controls loudness, while modulator level and its
frequency relationship to the carrier shape the overtones. Separate envelopes let
brightness decay faster than the body. Integer ratios are useful starting points
for pitched bass; inharmonic ratios can introduce clang. A sustained low carrier
plus a short modulation transient is a useful next FM-bass experiment; electric
piano requires its own decaying partial structure and velocity response. Adding
FM alone does not establish reggae authenticity. The existing six-operator engine
can support this work without inventing a second DSP implementation.

[Casio's account](https://note-pr.casio.co.jp/n/n743ca6a0e8c2?hl=en)
identifies the MT-40 connection to Sleng Teng. Early digital dancehall should not
be treated as synonymous with DX-style FM. DIGITAL BASS remains subtractive and
is not a replica of that keyboard or the user's references.

## Implemented changes

| Sound | Revision 0.2 | Reason |
| --- | --- | --- |
| Roots Organ | WHEEL 838000000; click 10/127, drive 3/127, percussion/rotary/sends off; full sustain, release 20 | Restore the missing 8′ fundamental and low harmonic body; dry, sharply gated chords |
| Rocksteady | WHEEL 888000000; soft third-harmonic percussion, click 20, drive 5; rotary/sends off; release 18 | More compact registration and less brittle upper-harmonic emphasis; distinguish attack from sustained tone |
| Deep Roots | Single saw through low-pass; cutoff 52, resonance 0, drive 4, keytracking 96; ADSR 14/98/18/43, filter envelope 8 | Retain useful low harmonics while reducing buzz; plucked decay instead of near-constant sustain |
| Digital Bass | Square, cutoff 48, resonance 0, drive 4, keytracking 64; ADSR 9/90/0/34, filter envelope 12 | Decaying electronic attack with less resonant sweep and distortion |

Numbers above are firmware values, not milliseconds. The envelope-time mapping is
1 ms × 10000^(value/127); exponential decay/release reaches approximately 99% of its
target at that time. Deep Roots decay is about 1.22 s to its 14% sustain, with a
roughly 265 ms time constant. Its release is about 23 ms. Organ release values
20/18 correspond to roughly 4.3/3.7 ms. All remain editable and saveable.

The HOME knob mappings and stored engine/preset indices remain unchanged. Existing
upstream presets and DSP are untouched. Bass patches retain the existing legato
mode: separated notes retrigger; overlapping notes connect. MONO mode for every-note
retrigger is available on the voice editing page. MIDI pitch is not transposed by
a hidden octave offset: on the virtual keyboard use OCT− twice for bass register.
Audition bass on headphones or speakers capable of reproducing low frequencies.

## Audition procedure

Compare dry sounds at similar perceived loudness, using the same notes and durations.
For bass test E1–G2, short notes, held notes and overlapping transitions. For organ
test close voicings in the middle register, repeated detached chords and alternating
short strokes; avoid assessing it only with a sustained block chord. Then compare
within a rhythm arrangement. Published demo phrases are original and are not the
melodies or basslines of the reference recordings.

Revision 0.1 WAVs are retained locally in build/reggae/v01-before/. Revision 0.2
renders use the actual firmware and are included with the emulator as audition
files. The organ demo now tests alternating two-/three-note strokes. Demo rhythm
changes mean those musical files alone are not a controlled timbre A/B; the fixed
MIDI test renders provide the controlled synthesis comparison.

Next priorities: user reference audition, piano chops, an FM bass candidate with
independent body/brightness envelopes, and more realistic plucked-bass behaviour.
Do not call the library authentic or reference-matched until listening supports it.
