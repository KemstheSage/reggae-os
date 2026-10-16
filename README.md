# REGGAE-OS

An experimental reggae instrument firmware for the **M-VAVE FM-1**, based on
[Felucca 1.6.1](https://github.com/hugelton/Felucca). The browser is a hardware
emulator running the same C synthesis and screen renderer, not the product.

Sound bank 0.3 supplies **13 playable presets**, including organ, bass, reed
melodica, electric piano, piano chop, brass stabs, muted string skank, dub siren
and an eight-piece drum kit. Four-knob editing and animated 240 × 240 illustrations
run inside the firmware. It preserves the original engines,
MIDI paths, effects, sequencing and flash-backed user-preset system.

**Early development build. Physical hardware and iPhone Safari are untested.**
The expanded sounds are audition candidates; musical refinement is ongoing.
See [TESTING.md](TESTING.md) for measured results and known limitations, and the
[bank 0.3 milestone report](docs/reports/bank-0.3.md) for screenshots and scope.

Sound bank 0.2 revises the registrations, dry organ articulation and plucked bass
envelopes following musician feedback. The listening brief is Big Ship, Faded Away
and Soldiers; these patches are audition candidates, not verified recreations.
See [SOUND_DESIGN.md](SOUND_DESIGN.md) for sources, exact changes and limitations.

## Play on iPhone or desktop

Open **https://kemsthesage.github.io/reggae-os/** in Safari and tap **Switch on**.
This HTTPS deployment runs the same firmware WASM as the local emulator.
Actual iPhone audio and multitouch operation still need device verification.

## Play the local emulator

After [building](BUILDING.md):

```sh
python3 -m http.server 8790 --bind 127.0.0.1 --directory build/emu
```

Open **http://localhost:8790/** on the Mac and click **Switch on**.
Play the onscreen FM-1 keys (or the keyboard shortcuts shown below the panel).
Drag knobs upward/right, use the mouse wheel, or focus a knob and use arrow keys.

- Starts on Roots Organ. On HOME, PRESETS cycles the thirteen curated sounds.
- Organ knobs: registration, upper harmonics, rotary speed, drive.
- Bass knobs: cutoff, resonance, filter-envelope amount, drive. Use OCT− to play low notes.
- ROOTS KIT maps MIDI notes 36/38/39/42/46/45/37/56 to kick, snare, clap,
  closed/open hi-hat, tom, rim and bell. It is a playable kit, not a new sequencer.
- Fresh settings use the REGGAE palette throughout menus. Existing users can select
  REGGAE in MENU → PALETTE; saved preferences are retained.
- EDIT and ENV expose the existing deeper parameters; FX retains Felucca effects.
- SAVE retains the original user-preset workflow. Held HOME opens the menu;
  MENU ANIM OFF reduces performance-screen motion. Existing factory patches retain their original HOME controls.

The public GitHub Pages link above works independently of this Mac. An iPhone
cannot use the Mac's `localhost`; use the HTTPS deployment instead. The original
emulator already provides touch controls and a rearranged phone-sized hardware
panel. Actual mobile Safari audio, touch polyphony and interruptions still require testing.

## Architecture and project files

- [HARDWARE.md](HARDWARE.md): display, memory, controls, build status and installation/recovery references.
- [SOUND_DESIGN.md](SOUND_DESIGN.md): synthesis decisions and musical limitations.
- [UI_DESIGN.md](UI_DESIGN.md): shared firmware artwork and rendering budget.
- [TESTING.md](TESTING.md): baseline, audio, emulator and hardware evidence.
- [ROADMAP.md](ROADMAP.md): remaining milestones.
- `firmware/src/reggae_engines.c`: appended presets and adapters over unchanged upstream DSP.
- `firmware/src/reggae_ui.c`: native screen artwork and animation.
- `tools/reggae/`: table generation, tests and source/licence packaging.
- `.github/workflows/reggae.yml`: proposed CI build/test artifact workflow; not yet run on GitHub.

The original README is retained in [docs/FELUCCA_README.md](docs/FELUCCA_README.md).
Upstream source and engines are pinned on the `reggae-os` development branch.

## Licence and credit

GPL-3.0-only as a combined firmware. Preserve [LICENSE](LICENSE),
[LICENSING.md](LICENSING.md), [CREDITS.md](CREDITS.md), `LICENSES/`, fonts and sample
attributions. Felucca is by Leo Kuroshita / Hügelton Instruments and contributors.
Felucca-Engines remains an unchanged MIT submodule; its preset/descriptor prefixes
are reproduced with attribution in the REGGAE-OS adapter to preserve stored IDs.

When distributing builds, include complete corresponding source, including the
submodule's contents. `tools/reggae/package-emulator.sh` prepares that alongside
the emulator. No proprietary TE artwork or commercial recordings are included.
M-VAVE, FM-1 and Teenage Engineering product names belong to their respective owners;
this project is independent and has no claimed endorsement.
