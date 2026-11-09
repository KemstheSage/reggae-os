# Drum redesign research — 9 November 2026

Scope: source/documentation research, producer/drummer interviews and design decisions.
Not a listening test of the third-party firmwares or commercial recordings. No code,
samples or firmware were installed on the FM-1 during this research. The current
Roots kit is rejected by the musician; numerical audio tests did not establish its
musical quality. Approved non-drum instruments remain protected.

## Firmware comparisons

- **X0X (FM-1):** its README describes a hybrid 909 engine: modeled drum bodies,
  sampled hats/cymbals; a separate 808 engine and user break slots. This is the most
  directly relevant evidence that sample hats are feasible on this hardware.
  X0X has a different overall allocation of resources; it does not prove its whole
  engine will fit alongside ours. Investigate a small hybrid hat/rim component,
  not replacing Riddim OS or importing another sequencer.
  Source: https://github.com/charlesvestal/fm1-x0x/blob/main/README.md
  Manual: https://charlesvestal.github.io/fm1-x0x/manual/
  X0X identifies its ER-99 cymbal samples as GPL-3.0 and credits Matthew Cieplak;
  check actual asset provenance and licence before copying any bytes.

- **SLOOP (FM-1):** current README documents 37 kits, including five sampled
  treatments and 32 synthesized kits. Its drum generator defines independent
  pitched/noise sources, envelopes and filters per hit. Boom-bap, lo-fi and ambient
  definitions differ beyond names. Level matching and darker soft hits are useful
  principles. Its licensing document identifies a CC0 acoustic kit under
  assets/samples-cc0/KIT; inspect individual attribution records before reuse.
  Source: https://github.com/isod89/sloop-fm1
  Definitions: https://github.com/isod89/sloop-fm1/blob/main/tools/gen_drumkits.py
  Licensing: https://github.com/isod89/sloop-fm1/blob/main/LICENSING.md
  This does not mean SLOOP's presets are already ported or sound-approved.

- **Mutable Instruments Plaits:** hi_hat.h offers six-square metallic noise,
  alternative ring-modulated sources, band-pass coloration and added clocked noise.
  Its file carries an MIT permission notice. It is evidence against the simplistic
  claim that all metallic hats are bad. Spectral balance, envelope and source mix
  matter. A wholesale floating-point port needs FM-1 performance measurements.
  Source: https://github.com/pichenettes/eurorack/blob/master/plaits/dsp/drums/hi_hat.h

- **Sonic Potions LXR:** dedicated drum synthesis includes pitched/noise snare
  components, FM percussion and a hat voice with separate open/closed decays and
  closed-hat choking. Useful particularly for an optional electronic/ambient kit.
  Different hardware (Cortex-M4); not a drop-in FM-1 engine. Repository availability
  alone is not licence clearance: exact source-file terms must be checked before
  reuse; no LXR code is imported here.
  Source: https://github.com/SonicPotions/LXR
  Manufacturer manual: https://www.sonic-potions.com/public/SonicPotions_LXR_OwnersManual.pdf

## Musical evidence and interpretation

Sly Dunbar describes choosing articulation and rolls in relation to tempo, chords
and microphone sound, and deliberately pursuing his own snare identity. That rules
out treating one generic snare or a preset label as "authentic reggae". His and
Robbie's RBMA interview also provides a practitioner perspective on developing a
rhythm-section sound. Earlier user references remain Big Ship, Soldiers and Faded
Away; they are future A/B references, not recordings measured in this research.
https://unitedreggae.com/articles/n1001/060212/interview-sly-dunbar-part-2-new-projects
https://www.redbullmusicacademy.com/lectures/sly-and-robbie-twins-peak/

Questlove's discussion of Dilla describes human-feeling drum programming. Our
interpretation: velocity, articulation and the relationship between lanes matter;
a global swing amount or random timing is not a universal hip-hop recipe.
https://www.redbullmusicacademy.com/lectures/uestlove-phrenology-of-hip-hop/

Native Instruments' lo-fi tutorial and Ableton's LoFi HipHop description emphasize
feel and sound texture. Our target is optional bandwidth/saturation treatment on
already useful hits, not mandatory crackle, excessive bitcrushing or loss of kick
and snare definition. Ambient is broad and may have no drums; the soft percussion
kit below is a design choice, not a claim about every ambient record.
https://www.native-instruments.com/blogs/blog/how-to-make-lofi-music
https://www.ableton.com/en/packs/lofi-hiphop/

## Proposed distinct sonic targets (not implemented)

| Kit | Kick / snare | Hats / percussion | Treatment |
|---|---|---|---|
| Roots Acoustic | Rounded controlled kick; woody cross-stick plus a separate snare with body and brief wire texture | Short cymbal-like closed articulation; open hat from the same family, clean choke; useful shaker | Dry first, modest colour; dub sends remain optional |
| Dusty Hip-Hop | Punchy short kick; layered body/crack snare, articulate at ghost velocities | Small, tight hats; restrained open hat; varied accents | Mild saturation, separate transient/body control where affordable |
| Soft Lo-Fi | Rounded kick; muted snare/rim preserving the attack | Soft hat/shaker texture without a persistent pitched ring | Optional darker filtering and restrained sampler character |
| Ambient Percussion | Gentle low thump; sparse click/brush-like hits | Soft noise, wood and selected tuned metallic hits | Longer controlled tails; reverb as a send, not baked into every sample |

These are working briefs, not claims that genres have a single required sound.

## Decision and FM-1 feasibility

Prioritize a **hybrid acoustic Roots candidate**, especially hats and cross-stick,
using clearly licensed short one-shots, with synthesis retained where it gives
musical control. Compare with a filtered-noise hat candidate rather than assuming
noise alone is realistic. Keep the dedicated electronic kits as a separate choice.

Hardware build correction: 0.8.1 application is 578948 bytes, but the actual
XIP linker region is **0x8DFBC**, not the 0x93000 package budget. Only about 2.6 KiB
remains. Embedding four short recordings overflowed XIP by 15864 bytes. The new
8-zone RDMDRM2 pack instead occupies 33258 bytes in an existing 80 KiB user slot.
No flash repartitioning. The emulator installer only uses an entirely erased slot
(or reuses an identical pack), never replacing another user sample. Missing pack
falls back to synthesis. Hardware installation is deferred until auditions.

Do not silently change the stored drum model IDs used by existing beats. Append a
new model/preset when changing DSP identity; preserve previous settings and the
verified hardware backup. Keep GM lane notes and the eight-lane grid compatible.

## Acceptance process

1. Check source licences and compare original/transcoded sample quality.
2. Render old and candidate hats at identical musical settings, with clear gain
   matching; provide isolated hits, closed/open choke phrases and complete grooves.
3. Include varied velocities, fast rolls and slow beats. A velocity change should
   produce musically sensible articulation, not merely a quieter copy where feasible.
4. Judge hats in a mix as well as solo; snare/rim must retain body and kick must leave
   room for bass. Do not hide an unsatisfactory dry sound behind effects.
5. Check clipping, DC, tail discontinuities, repeat stability, voice stealing,
   runtime budget and firmware size. These checks establish safety, not taste.
6. Publish audition candidates before another hardware flash. User approval of the
   sound is the acceptance criterion; preserve approved organ/piano/bass golden audio.
