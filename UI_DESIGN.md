# Hardware visual language

The sole performance UI is C in `firmware/src/reggae_ui.c`. Both hardware and
WebAssembly include it via `ui_draw.c`. The browser only scales the RGB565 screen
and exposes existing panel controls. There is no separate browser synthesizer.

240 × 240 layout: identity/preset at y=0–55, illustration y=56–179,
four equal control columns and navigation/message line y=180–239.
Ink/charcoal with restrained ochre, green and vermilion accents evokes painted
cabinets and printed labels. Original geometric artwork; no TE assets or branding.
The [V&A music exhibition guide](https://www.vam.ac.uk/articles/the-music-is-black-large-print-guide)
provides historical sound-system and print-culture context. This first artwork is
an engineering prototype, not a completed survey of Jamaican graphic traditions.

Organ: nine drawbar positions reflect actual registration plus SUB/BODY/TOP offsets;
the percussion-disabled top bar is reflected. Rotary pointer reads actual WHEEL
rotor phase, including speed ramping. Keys react to measured track audio level.
Bass: four speaker cabinets react to the existing track peak meter with visual decay.
This is an amplitude indicator, not a drawing of every low-frequency oscillation.
The four knob pointers and values always reflect the assigned parameter; bass
cutoff and drive currently change these indicators, not the cabinet geometry.

Bank 0.3 follows the supplied Direction 3 reference using original integer-drawn
artwork. Reed airflow follows note level and vibrato phase/depth; keyboard hammers
and brass bursts follow track amplitude; plucked strings move with amplitude;
siren trajectory follows the actual LFO phase. Drum strike highlights follow
actual voice activity for each of the eight GM lanes. Key highlights represent
level, not an exact map of the held notes.

FX, DLY and REVERB pages have shared native illustrations. Delay repeat spacing
reflects the time-division selection and marks reflect feedback. The record spins
as an illustrative mechanism, not a measured tape or audio transport. Reverb coil
height reflects damping and its motion amplitude reflects size and track level;
it is not an analysis of the wet effect tail. Reverb TYPE retains ROOM/SPRING/HALL;
the coil is a metaphor and does not turn every algorithm into spring reverb.

10 Hz upper bound, no heap allocation, integer drawing, existing strip buffer.
Rendering occurs in the main/UI loop, never in the audio callback. MENU ANIM OFF
freezes time-driven rotor, cone and LFO motion; values and note activity remain
live. Full 240 × 240 updates at 10 Hz require about 1.15 MB/s before SPI overhead
against the nominal 1.5 MB/s link. Dirty rectangles and hardware timing measurements
remain outstanding; no additional full-screen framebuffer is allocated.

The boot screen uses cream typography/artwork. Preset lists, settings, SAVE,
confirmations and advanced editors retain their tested upstream layouts with the
new REGGAE palette. An illustrated category grid and bespoke layouts for every
advanced page remain unfinished. Saved user patches use upstream HOME controls
because their engine alone cannot identify whether a patch is bass, reed or brass.

## UI 0.3.1 — researched interaction refinement

See [the OP-1 field / Sidekick study](docs/research/te-interface-study.md) for official
sources and the distinction between observations and our own design decisions.
The current prototype implements four distinct control colours, musical labels,
and an enlarged value/range view lasting 1.2 seconds after an encoder turn. It
returns automatically to the instrument illustration; changing preset/page also
hides it. This is native C drawing shared by firmware and emulator. Audio patches
and control assignments are unchanged. Analogue bass now shows a schematic filter
knee/crest that responds to cutoff/resonance. It is not a measured frequency plot.
Further work includes category browsing, effect engagement states and artwork
refinement; this is not a complete TE-style interface replacement.

## UI 0.3.2 — artwork quality

The display remains 240 × 240. Quality comes from stronger shapes, controlled
material colours and edge coverage, not a browser-only high-resolution substitute.
Original organ artwork now has a rounded console, inset controls, graduated
registration rails, two shaded manuals and an integrated rotary cabinet. Bass
cabinets have recessed baffles, layered cones, corner fittings and restrained
wood-toned edges. Piano, brass and siren housings also gain fuller shapes.

Native circles use 4 × 4 coverage samples only around their boundary; interiors
use filled spans. Sloping lines use integer coverage blending. Both composite
into the existing strip buffer; no floating-point graphics, heap allocation,
additional framebuffer or TE bitmap assets are introduced. The same helpers
smooth the rest of the instruments, effects and knob rings.

Speaker motion has a bounded visual gain so normal playing levels produce visible
movement. It still follows the track peak envelope, not a decorative oscillator.
The drawbar registration and actual rotary phase remain connected. The 10 Hz cap
and ANIM OFF behaviour remain. Real hardware timing is still unmeasured; a build
pass cannot prove that the extra boundary calculations leave enough audio headroom.

[Native before/after comparison](docs/images/ui-0.3.2-comparison.png) ·
[Full current bank](docs/images/ui-0.3.2-bank.png).

## UI 0.3.3 — melodica character

The reed player has a clearly drawn face, closed eyes, ribbed cap, rounded body,
and a mouthpiece visibly connected to the melodica. Mouth opening and cheek
expansion use the existing smoothed audio peak, sampled by the 10 Hz UI. The mouth
closes below the activity threshold and opens progressively to eight pixels.
Breath lines appear with that same activity; pitch-LFO phase/depth shapes their
wave. This is an expressive level illustration, not a breath-pressure sensor.
ANIM OFF holds the mouth closed and hides breath lines. No DSP changes or new
allocation; all artwork is shared native firmware code. This character treatment
is implemented for melodica; additional character identities are future work.

## UI 0.3.4 — instrument creatures (supersedes the human reed player)

A shared family uses cream eyes, dark pupils, restrained colours and musical
movements. The melodica is now a keyboard creature with a bellows-like body:
fold spacing and mouth opening follow the smoothed audio level. Bellows are an
expressive breath metaphor, not a claim about melodica construction. Vibrato moves
its gaze. The siren's pupils follow its pitch LFO and its mouth follows amplitude.
Each drum head opens its own mouth when a voice assigned to that lane is active.

The guitar stays an instrument illustration: two body bouts, a waist, soundhole,
bridge, aligned fretboard, six strings and three-a-side tuners. String movement
uses audio activity. It remains the same synthetic plucked-string patch.

The FX page replaces four bar columns with a toothy drive box, paired chorus
companions, an echo record and a coil-shaped reverb creature. Amount strips stay
visible. Drive changes the jaw/teeth, chorus amount changes separation in motion,
echo send adds marks, and reverb send scales activity. Detailed echo uses the real
clamped tempo/division delay length for rotation cadence and repeat spacing;
feedback changes repeat count, colour changes their palette. These are parameter
illustrations, not a measured wet-signal display. Room/Hall use architectural
bodies on the detailed reverb page; only Spring uses the spring body. Size and
level scale movement and spring damping changes its width.

All rendering remains integer geometry at 240 × 240, capped at 10 Hz, using the
existing strip buffer. ANIM OFF freezes time-based motion; control changes and
static status indicators still update. No DSP, control mapping or presets changed.

## UI 0.4.0 — pattern and arrangement workspace

STEP and SONG now share the instrument pages' charcoal/cream/gold/green/red
palette, top identity, central working area and four colour-coded bottom controls.
The established piano roll and eight-lane drum grid retain their notes, locks,
ties and transport playhead. The arrangement replaces compressed blocks with
four readable section rows showing T1–T4, A–D or mute, selected cell and loop count.
A green marker follows actual song-section playback, including scroll to later
sections. These are state indicators, not free-running decorative animation.
ANIM OFF does not suppress essential transport position. Three existing scratch
strip renders at 10 Hz; no full framebuffer or audio-callback drawing.

DETAIL, automation, mixer, project browser and held-button tools retain their
upstream layouts. See SEQUENCER.md for the actual hardware workflow and limits.

## UI 0.4.1 — simple pads

Main STEP now shows a four-by-four bank of large numbered pads, replacing the
piano roll and simultaneous eight-drum display. A pad shows its first note
(with + for chords), HOLD for a tie, or HIT for the selected drum lane. Cream
outlines mark editing; green underlines follow actual playback. Accent and lock
markers remain. Steps beyond length are dim. Existing four knobs, keyboard,
recording, transport and deeper DETAIL/AUTOMATION pages are unchanged. Titles
and hints use task language: MAKE A PATTERN, MAKE A BEAT and SONG.

## UI 0.4.2 — restored editor and held-button note entry

Restores the UI 0.4.0 piano roll/full drum grid and ARRANGE naming. On the stopped
melodic STEP page, EDIT+keys adds, REC+keys replaces and EDIT+REC clears. The
cursor stays fixed; contextual hints appear while held. Key presses continue to
audition the sound instead of selecting engines/recording settings. Modifier
releases are consumed, including keys held after a modifier release. Quick REC
taps retain arming. Drum and deeper editing controls are unchanged.

UI 0.5.0 keeps the instrument animations and replaces HOME's decorative top-right
marks with four track numbers. Gold identifies the selected track, red identifies
recording, ready blinks at 2 Hz, and beat marks follow sequencer position. The
bottom hint prioritises READY/RECORDING until the take is finished. Rendering
remains limited to 10 Hz using the existing strips; no graphical work enters DSP.

UI 0.5.1 gives SEQ an original miniature 45 label, green/gold/red print marks,
warm notes against dark green lanes, red selection/playhead accents, four beat
lights and cabinet-style parameter wells. The record spindle advances with the
selected track's sequencer position; it stops when transport stops and respects
reduced-animation settings. The established roll/grid renderer, note ties,
ratchets, parameter-lock marks and editing geometry are retained. A local palette
is restored after drawing, so other pages keep their own colours. No DSP changes.

UI 0.5.2 turns the SEQ record into an original instrument character: eyes, a
smiling/open mouth, tiny hands and feet. The selected track's active firmware
voices open its mouth, including release tails; silence closes it. The arrangement
version responds to voices across the four tracks. Outer-groove rotation follows
sequence position while its face stays upright; it blinks when idle and looks
across the beats. Animations-off removes blinking, looking and bobbing while
keeping the mouth as a useful voice-activity indicator. No audio callback changes.

## UI 0.5.3 — tabletop sequencer

The piano roll, drum grid and arranger now sit inside a shallow metal machine on
an illustrated wooden desk. A bevel, cast shadow, cable, rubber feet and screws
establish the physical object. Four rotary indicators follow the displayed step,
note/drum, length/hit and feel/accent values (or arrangement controls). The record
character retains its voice-driven mouth and beat-driven motion.

Rendering remains shared native C at 240×240 and up to 10 Hz. The grid removes
one blank column per step to fit the case, retaining native font and note pixels;
the drum selection outline is redrawn at the new spacing. One temporary 480-byte
row buffer replaces neither the existing strip buffer nor any audio buffer.
Editing gestures, sounds, sequencing and storage are unchanged.

UI 0.5.4 corrects the tabletop layout: the complete record character stays within
its own header bay, title/status/preset lines have separate vertical bands, and
the help strip sits inside the extended case. Values have taller inset wells.
Long values, preset names and help messages are fitted with ellipses at the native
font size rather than overflowing adjacent controls or the case.

UI 0.5.5 uses the existing small font for instrument category and preset names on
the performance screen (9-pixel capital height instead of 11). Baselines remain
optically centred in the header bands, with long preset names bounded to 216 px.
Effects headings and the already-small sequencer preset labels are unchanged.
