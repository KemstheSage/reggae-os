# Milestones

1. Foundation: upstream v1.6.1 and pinned engines cloned; architecture investigated;
   baseline hardware/emulator builds complete. Full test runs completed with documented
   baseline failures; see TESTING.md.
2. Identity: implemented first hardware-native organ/bass performance screen,
   branding, four parameter columns and bounded animation. Broader screen polish remains.
3. Organ/bass: implemented four new synthesis patches and direct knob assignments;
   audible-output, release, persistence and native/WASM parity checks passed.
   Desktop emulator verified. Roots Organ 0.2 audition approved as the starting point
   on 15 October 2026; preserve it and develop separate organ variants. Other
   presets still need musician approval; physical testing remains.
4. Extended instruments: not implemented as REGGAE-OS instruments. Existing Felucca
   FM6, PHYS, SAMPLE and other engines remain accessible. Next: musician audition
   and organ/bass refinement before melodica, electric piano, brass, skank and siren.
5. Dub effects: retained upstream effects; dedicated performance mapping/graphics pending.
6. Presets: retained flash storage, full browsing and save/recall. Four-sound HOME bank
   implemented; genre banks and at least twelve curated instruments pending.
7. Hardware optimisation: no physical device. Linker and target builds are necessary
   but cannot establish DAC quality, timing, controls, flashing or recovery on hardware.
8. Final polish: pending auditions and hardware validation.

This repository is an early instrument prototype, not the complete final vision.
