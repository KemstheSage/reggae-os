# Interface study: OP-1 field and EP-136 K.O.-sidekick

Research date: 16 October 2026. Scope: official manuals and screen illustrations,
not hands-on testing. Distinguish OP-1 field from the original OP-1. No TE images,
fonts, icons, screen layouts or code are incorporated into REGGAE-OS.

## Sources and observations

1. [OP-1 field official guide, firmware 1.7.0 REV 10](https://assets.teenage.engineering/_img/69f248938d433104c4dd1846_original.pdf),
   PDF pages 8–11, 14, 16 and 27 (one-based). Colour links encoder and parameter;
   engine, envelope, effects and modulation have separate views. Dimension's
   waveform/filter drawing and DNA's symbolic graphic demonstrate distinct
   engine identities. Exact values/pop-ups coexist with graphical representations.
   The field supports encoder presses; that is not an FM-1 assumption.
2. [Sidekick main screen and controls](https://teenage.engineering/guides/ep-136/getting-to-know-your-sidekick),
   sections 6.1–6.9. The official overview drawing separates channel meters,
   compression/gain markers, tempo, cue and FX states. Focused channel/compressor
   views expose detail. Performance effects can be momentary or locked.
3. [Sidekick effects](https://teenage.engineering/guides/ep-136/fx), sections 7.1–7.6.
   Controls have specific roles within each effect; the delay illustration combines
   division text with simple geometric repeat symbols. Force pad/mod stick gestures
   depend on its hardware and must not be transplanted to an FM-1 UI.

These are manufacturer descriptions/static illustrations. Animation timing and
usability on either TE device have not been independently measured here.

## Original REGGAE-OS direction

The central lesson is a visible relationship between action and sound, supported
by a quiet overview and readable editing detail. Preserve the user's Direction 3
cream/charcoal sound-system aesthetic and original Jamaican instrument artwork.
Use a few deliberate shapes rather than detailed decorative scenery.

| Principle | REGGAE-OS application | Status |
|---|---|---|
| Control and graphic share an identity | Left-to-right ochre, green, cream, vermilion; labels and positions remain usable without colour | Implemented UI 0.3.1 |
| Focus on the parameter being edited | Turning a physical/virtual knob briefly shows a large exact value and range marker, then returns after 1.2 s | Implemented UI 0.3.1 |
| Describe musical purpose | HARM, TONE, ROTOR, DRIVE; TIMBRE/ENV instead of raw FM macro abbreviations | Implemented for curated bank |
| Graphics explain parameters | Bass filter knee follows cutoff; crest follows resonance; existing organ rotor and drawbars retain real state | Implemented, schematic not measured response |
| One recognizable object per instrument | Organ mechanism, speaker cabinet, reed, string, horn, siren, drum strikes | Existing; simplify/refine further |
| Distinguish selected, engaged and bypassed | Dedicated FX indicators using actual wet/enable state | Further work |
| Stable navigation | Existing HOME, EDIT, ENV, LFO, FX and SAVE controls; no invented touch/encoder-click hardware | Preserved |
| Instrument before workstation | An original category browser with a small icon and readable preset list | Further work |

The focused view is our adaptation, not a claim that TE automatically opens the
same type of view on every knob turn. It works on the FM-1 without an extra key.
The range bar is a parameter position, not an audio level meter. A zero marker is
shown for bipolar controls. Named values remain named; numerical values keep the
firmware descriptor's units. Envelope macros remain macros, not invented times.

## Animation contract

- Play: amplitude drives cones/strikes; actual rotary/LFO phase drives motion.
- Edit: knob changes update the large value/range immediately on the next UI draw.
- Cutoff/resonance: schematic filter shape follows the stored parameters, including
  when ANIM OFF is selected; this is necessary feedback, not decorative movement.
- Effects: eventual engaged/bypass/tail displays must use actual effect state. Do
  not label input-envelope movement as measured reverb decay.
- Instrument changes should be immediate; no long transitions that delay playing.

## Hardware boundaries

Render at 240 × 240 using the existing 240 × 124 RGB565 strip. Keep 10 Hz cap,
integer drawing, no extra framebuffer and no graphics in the audio callback.
The encoders are not colour-coded on the physical FM-1: position and labels are
primary, screen colours reinforce them. OP-1 field's display aspect and Sidekick's
portrait grid do not determine the FM-1 layout. No TE pad, mod stick, encoder press
or unsupported sensing is assumed.

## Later independence

The user has selected a future implementation with no Felucca code. That rewrite
is deferred. This pass is a prototype in the current Felucca-based firmware;
it does not remove its dependency or licensing. Preserve this UI specification
and original artwork as product requirements, then assess which implementation
assets can be reused in the independent system rather than assuming a code copy
would satisfy the no-Felucca requirement.
