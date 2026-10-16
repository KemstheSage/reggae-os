# REGGAE-OS bank 0.3 — 16 October 2026

Playable firmware milestone: 13 curated sounds, four controls each, original native
240 × 240 illustrations, eight-lane drum kit and shared effects screens.
The approved Roots Organ remains byte-identical in the original audition.

- **Implemented:** nine additions to the original four sounds; shared bank metadata;
  factory-load modulation settings; themed boot, performance and effects screens;
  REGGAE palette for existing settings/editors; original eight-strike auditions.
- **Main files:** `firmware/src/reggae_engines.c`, `reggae_ui.c`, `ui.c`, `ui_draw.c`,
  `eng_fm6.c`, `eng_phys.c`, `eng_drum.c`, `engines.c`, `category.c`, `panel.c`;
  palette generator, editor mock, emulator cache version, tests and documentation.
- **Builds:** WebAssembly and pi32v2 FM-1 firmware package. Application 517,516 B,
  data/BSS 61,000/73,728 B, pool 328,896/368,640 B. Hardware checks pass.
- **Tests:** 65 native sound/control renders; 65 bit-exact native/WASM comparisons;
  finite, bounded, non-silent PCM; repeated-note release; NOR save/recall including
  FM6 patch bytes; nine eight-strike auditions including all drum lanes; approved
  organ WAV comparison; web editor and upstream emulator regressions. Actual WHEEL
  rotor animation changes screen pixels; ANIM OFF freezes the illustration.
- **Visual evidence:** [native screens](../images/bank-0.3.png), each rendered at
  240 × 240 by firmware. Desktop browser start/preset navigation/effects inspected.
- **Incomplete:** musician approval of new sounds, custom illustrated category
  navigation, special layouts for all advanced pages, preserving custom performance
  identity after user-preset recall, echo throws and further sound/effects refinement.
- **Compatibility:** COMPILES and EMULATOR TESTED. Physical FM-1 audio timing,
  flashing/recovery and iPhone Safari remain HARDWARE UNTESTED/device unverified.
  Desktop CPU timing cannot establish embedded real-time headroom.
- **Access:** https://kemsthesage.github.io/reggae-os/?bank=0.3 — tap Switch on, then
  turn PRESETS on HOME through the thirteen sounds. Existing saved palette settings
  can be changed to REGGAE via MENU. The instrument itself remains firmware C.
- **Next priority:** audition/refine the new instruments and kit, complete instrument
  browsing, then reduce redraw bandwidth and validate with a physical FM-1.
