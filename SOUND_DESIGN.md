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

## Bank 0.3 — expanded audition candidates

The approved Roots Organ bank 0.2 is unchanged. Nine additions extend the four
existing sounds using the firmware engines, with no new sample library:

| Sound | Method | Four HOME controls |
|---|---|---|
| Reed Melodica | Filtered pulse/PWM reed approximation, slow attack, subtle delayed pitch LFO | Attack, cutoff, vibrato depth, release |
| Warm Tine | Existing six-operator tine patch with reduced modulator level and envelope macro, detune and chorus | Modulator level, envelope macro, chorus, detune |
| Piano Chop | Existing licensed piano sample, short envelope and mild drive | Cutoff, decay, drive, release |
| Brass Stabs | Detuned filtered oscillators and filter envelope | Cutoff, attack, filter envelope, release |
| Muted Skank | Existing PHYS string model, damping and excitation position | Brightness, damping, position, release |
| Dub Siren | Sine oscillator, sustained envelope, pitch LFO and echo send | Envelope pitch offset, LFO rate, delay send, cutoff |
| Round FM Bass | Existing six-operator FM bass patch with reduced modulation/envelope macros | Modulator level, envelope macro, modulator ratio, output level |
| Dub Drawbars | Separate dark WHEEL registration with slow rotary and echo | Registration, upper harmonics, rotary, delay send |
| Roots Kit | Existing TIGHT drum model, adjusted tone, decay, snap, accent and drive | Tone, decay, snap, drive |

The FM additions use the original factory operator patches with changed synthesis
macros. They are not new hand-authored six-operator algorithms. Melodica, brass and
guitar remain synthetic approximations, not authenticated acoustic recreations.
The siren's pitch control acts on synthesis (including external MIDI), not just
keyboard transposition. Echo HOME controls are send levels; feedback is on FX → DLY.

Roots Kit uses original synthesized drums: MIDI 36 kick, 38 snare, 39 clap, 42 closed
hat, 46 open hat, 45 tom, 37 rim and 56 bell. Keep the traditional one-drop or
steppers rhythm in the performance/external sequencer; a kit timbre alone does not
produce a reggae rhythm. No commercial recording samples are redistributed.
The new ten-second auditions play eight individual notes/chords or drum lanes,
with space to inspect articulation; they are test phrases, not finished arrangements.

## Bank 0.4 — five-patch audit, 21 October 2026

Scope: only Reed Melodica, Brass Stabs, Muted Skank, Dub Siren and Roots Kit.
All eight organ/piano/bass presets and their DSP are deliberately unchanged.
Forty fixed-MIDI renders (defaults plus four knob variants per protected preset)
are checked against SHA-256 hashes captured before editing. No global effects,
audio engine or instrument model changes. The new sound parameters use existing
save/recall paths; existing user-saved patches are not overwritten.

### Evidence and limits

- [Yamaha's Pianica explanation](https://hub.yamaha.com/winds/w-gearguide/pianica-a-great-way-to-start-a-musical-journey/)
  describes air routed across individual tuned reeds. Its
  [P-37E2 description](https://usa.yamaha.com/products/musical_instruments/winds/pianica/p-37e/features.html)
  describes breath flow and internal damping as contributors to tone. This supports
  a controlled reed spectrum and breathing articulation, not a noise-heavy flute.
- [SoundFingers' own reggae guitar manual](https://soundfingers.com/wp-content/uploads/2022/01/Le-Skank-User-Guide-v1.2.6.pdf),
  introduction and guitar setup, emphasises damped offbeat chords and distinguishes
  pickup brightness. It informs articulation; no audio or code from that product
  is used here.
- [Gordon Reid's brass synthesis study](https://www.soundonsound.com/techniques/synthesizing-brass-instruments)
  motivates envelope/spectral shaping. The exact settings below are our synthesis
  choices, not measured acoustic brass or settings supplied by the article.
- [Drumeo's performance lesson](https://www.drumeo.com/beat/musical-drumming-in-different-styles/)
  and its [reggae play-along description](https://www.drumeo.com/free-playalongs/)
  inform the cross-stick-led audition. Kit sound and a one-drop rhythm are separate.
- [Benidub's DS01](https://benidub.com/products/ds01-dub-siren/) provides a real
  dub-siren design reference. Our filtered oscillator and modulation remain a
  firmware approximation, not an emulation of that circuit.

There has been no verified subjective listening or direct comparison against the
user's three commercial references in this audit. Numerical tests establish
articulation, stability and preservation, not musical authenticity. These are
research-informed audition candidates requiring the musician's judgement. No
copyrighted recordings have been sampled or redistributed.

### Specific changes

| Patch | Adjustment | Intended result |
|---|---|---|
| Reed Melodica | PWM shape envelope −24; cutoff 73→79; noise 2→1; attack 32→36, sustain 112→116, release 34→43; retain ~5.1 Hz shallow delayed vibrato | Add even harmonics to the previously symmetric pulse, keep reed clarity, soften breath entry/exit without a pad-like attack |
| Brass Stabs | Attack 23→40 (~5→18 ms); decay 67→82; sustain 80→76; filter envelope 25→30, drive 7→4, release 26→37 | Brief blown attack with stronger brightness development, less distortion, clean releases; remains synthetic ensemble brass |
| Muted Skank | STRC 38→32 (neutral string stiffness region); brightness 62→78; pluck position 76→38; accent 80→52; slight pick excitation; ADSR 0/68/28/30 | Cleaner harmonic string body, sharper pick and shorter chord body, with less ringing while held. Three-voice PHYS limit remains: use compact three-note voicings |
| Dub Siren | Sine→filtered square; triangle pitch LFO, depth 24→40 (about ±7.5 semitones), rate 59→72 (~2.2 Hz), reduced drive and echo send | Richer sound-system wail and wider pitch movement. Rate/depth/waveform remain editable. No global delay changes |
| Roots Kit | Keep TIGHT model; restore designed tuning (57→64), tone 51→60, decay 48→55, snap 72→62, lower accent/drive; rebalance individual lanes | Preserve the dry short kit while giving kick/rim priority over hats and clap. It remains a synthesized electronic kit, not a sampled acoustic roots session |

Lane levels (0–127): kick 127, snare 114, clap 82, closed hat 92, open hat 86,
tom 110, rim 127, bell 94. MIDI 37 is the cross-stick/rim candidate. Drum note-off
correctly does not truncate one-shot hits; closed hat chokes open hat.

### Auditioning

The existing isolated phrases retain identical MIDI before/after and are the
controlled comparison. New `style-*` WAVs are original 80 BPM performance studies:
upper-voiced 100 ms guitar chops, short horn stabs, a melodic reed line, siren knob
moves, and kick/rim on beat three with light eighth-note hats. They do not add a
sequencer to the instrument. Play guitar around C4–C5, brass C4–C5, melodica G4–D5,
and siren around C5 before exploring wider registers. Preset names/indices stay put.

## Bank 0.5 — recorded roots trumpet and three sirens

ROOTS TRUMPET replaces the curated BRASS STABS entry with a recorded acoustic
trumpet, not a full horn section. The old ANALOG preset remains available and
saved patches retain their engine and preset numbers. CC0 VSCO 2 CE sustains
use C4/G4/D5 zones (the source names use C3 for middle C), 0.95 seconds each,
with crossfaded sustain loops beginning at 0.4 seconds. Measured source pitch
is compensated using the existing fractional root-pitch metadata. Tone, attack,
decay and release remain real SAMPLE/ADSR controls. The original tongued onset
is retained; modest room send, no chorus or distortion. Audition around C4–G5;
extreme transpositions will sound less natural. All chords remain freely playable.

DUB WAIL uses a sine oscillator, triangle pitch LFO and echo send. JUNGLE SIGNAL
uses a filtered pulse with a descending pitch envelope tied to its decaying
amplitude (zero sustain): a held key still produces a short signal. FALL sets
pitch-envelope amount, LENGTH sets decay, ECHO is delay send and TONE is cutoff.
FAST ALARM uses a filtered pulse and square pitch LFO; RANGE changes pitch span,
RATE changes alternation speed. The engine's existing square LFO has abrupt
pitch changes; this is an intentional alarm character. These are original
synthesis candidates, not copies of the sampled effects in named records.

Existing siren character art is reused; its mouth follows signal level and its
eyes follow LFO activity. No new browser-only graphics or audio code.

The expanded [reference brief](docs/reports/roots-horns-and-sirens.md) distinguishes
research from verified listening. Musical approval still requires auditioning;
automated checks establish technical behavior, not authenticity. All approved
organ/piano/bass patch definitions remain unchanged.

UI 0.6.0 keeps all fifteen curated sounds unchanged. Optional ROOTPERC adds
five VCSL CC0 acoustic attacks obtained from SLOOP's separately attributed
collection: side-stick, conga, claves, shaker and tambourine. They use the
existing SAMPLE user-slot decoder (44,637 bytes ADPCM), not a new drum engine.
The exact-key map, source chain and hardware upload procedure are in
`assets/samples-cc0/REGGAE_PERC/ATTRIBUTION.md`. GATE 1/16 and MONO are live
master-bus effects; they do not rewrite patches. A gate can give hats/percussion
rhythmic cuts; mono collapses stereo ambience for a contrasting throw/release.
