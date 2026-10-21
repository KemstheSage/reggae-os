# Bank 0.4 — targeted reggae sound audit

Only melodica, brass, guitar, siren and drums were changed. The user's approved
organs, pianos and basses are protected by forty before-edit render hashes.
UI 0.3.4's approved graphics remain unchanged.

Research, exact parameter adjustments, musical intent and limits are documented
in [SOUND_DESIGN.md](../../SOUND_DESIGN.md). These are original synthesis audition
candidates, not verified recreations of the user's reference recordings. No
subjective listening claim is made; musical approval remains with the user.

Changed: three preset source files (`reggae_engines.c`, `eng_phys.c`, `eng_drum.c`),
new performance audition generator, protection hash fixture/checker, test/package
scripts, emulator version/cache key and documentation. No shared DSP changes.

## Listen

Each musical study is 16 seconds. The before/current isolated clips use identical
MIDI and playback gain for a controlled comparison; do not confuse different
musical phrases with improvements in timbre.

| Instrument | Musical study | Controlled before | Controlled current |
|---|---|---|---|
| Melodica | [Play](https://kemsthesage.github.io/reggae-os/auditions/reed-melodica-style.wav) | [Before](https://kemsthesage.github.io/reggae-os/auditions/reed-melodica-before-bank04.wav) | [Current](https://kemsthesage.github.io/reggae-os/auditions/reed-melodica.wav?bank=0.4) |
| Brass | [Play](https://kemsthesage.github.io/reggae-os/auditions/brass-stabs-style.wav) | [Before](https://kemsthesage.github.io/reggae-os/auditions/brass-stabs-before-bank04.wav) | [Current](https://kemsthesage.github.io/reggae-os/auditions/brass-stabs.wav?bank=0.4) |
| Guitar | [Play](https://kemsthesage.github.io/reggae-os/auditions/muted-skank-style.wav) | [Before](https://kemsthesage.github.io/reggae-os/auditions/muted-skank-before-bank04.wav) | [Current](https://kemsthesage.github.io/reggae-os/auditions/muted-skank.wav?bank=0.4) |
| Siren | [Play](https://kemsthesage.github.io/reggae-os/auditions/dub-siren-style.wav) | [Before](https://kemsthesage.github.io/reggae-os/auditions/dub-siren-before-bank04.wav) | [Current](https://kemsthesage.github.io/reggae-os/auditions/dub-siren.wav?bank=0.4) |
| Drums | [Play](https://kemsthesage.github.io/reggae-os/auditions/roots-kit-style.wav) | [Before](https://kemsthesage.github.io/reggae-os/auditions/roots-kit-before-bank04.wav) | [Current](https://kemsthesage.github.io/reggae-os/auditions/roots-kit.wav?bank=0.4) |

Use factory preset recall to load the revised settings. Existing saved user patches
retain their stored settings. A held browser session may still contain an edited
old patch; select another factory preset and return to the wanted one.

Tests: native 65-case audio/knob/storage suite, protection check, individual drum
lanes, five musical studies, graphics regression, WASM parity and hardware build.
Final build measurements and remaining hardware limitations are in TESTING.md.
