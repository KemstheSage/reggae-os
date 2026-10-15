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
Bass: two speaker cones react to the existing track peak meter with visual decay;
filter rail reflects cutoff; high drive changes cone colour. This is an amplitude
indicator, not a drawing of every low-frequency oscillation.

10 Hz upper bound, no heap allocation, integer drawing, existing strip buffer.
Rendering occurs in the main/UI loop, never in the audio callback. Existing MENU
ANIM OFF freezes rotor and cone motion; bars/values remain live. The first version
redraws the illustration and parameter strip; narrower dirty rectangles and
hardware timing measurements are outstanding. No full-screen framebuffer added.

Secondary pages, SAVE, modal confirmations and layers keep upstream rendering.
The bespoke palette currently applies only to the organ/bass performance screen.
