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
