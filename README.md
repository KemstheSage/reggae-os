# REGGAE-OS

An experimental reggae instrument firmware for the **M-VAVE FM-1**, based on
[Felucca 1.6.1](https://github.com/hugelton/Felucca). The browser is a hardware
emulator running the same C synthesis and screen renderer, not the product.

Sound bank 0.5 supplies **15 playable presets**, including organ, bass, reed
melodica, electric piano, piano chop, sampled roots trumpet, muted string skank, three dub/jungle sirens
and an eight-piece drum kit. Four-knob editing and animated 240 × 240 illustrations
run inside the firmware. It preserves the original engines,
MIDI paths, effects, sequencing and flash-backed user-preset system.

UI 0.5.0 begins the SLOOP-style recording integration: on HOME, arm with REC,
play the first note to start a loop at the set tempo, then overdub other tracks.
Fresh sessions start with organ, bass, trumpet and drums. Free-tempo capture and
live section recording remain planned. See [the song-making guide](SEQUENCER.md)
and [integration scope](docs/reports/ui-0.5.0.md).

UI 0.5.3 gives the sequencer a tabletop-machine case, inset grid, responsive rotary
indicators and animated record companion. [See the native-screen preview](docs/reports/ui-0.5.3.md).

**Early development build. Physical hardware and iPhone Safari are untested.**
The expanded sounds are audition candidates; musical refinement is ongoing.
See [TESTING.md](TESTING.md) for measured results and known limitations, and the
[bank 0.3 milestone report](docs/reports/bank-0.3.md) for screenshots and scope.

Sound bank 0.2 revises the registrations, dry organ articulation and plucked bass
envelopes following musician feedback. The listening brief is Big Ship, Faded Away
and Soldiers; these patches are audition candidates, not verified recreations.
See [SOUND_DESIGN.md](SOUND_DESIGN.md) for sources, exact changes and limitations.

## Sequence and arrange

UI 0.4.2 restores the piano roll and full drum grid. On the stopped note editor,
hold EDIT and play keys to add notes, hold REC and play to replace, or hold
EDIT + REC to clear the selected step.
Use SEQ for patterns and GLO for ARRANGE. Four parts can play a sixteen-section
song from four saved pattern sources, with per-track mutes and section repeats.
See [the song-making guide](SEQUENCER.md) for controls, saving and limits.

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

- Starts on Roots Organ. On HOME, PRESETS cycles the fifteen curated sounds.
- Horn knobs: tone, attack, decay and release. ROOTS TRUMPET uses three CC0 recorded notes.
- Sirens: DUB WAIL, JUNGLE SIGNAL (fall/length/echo/tone), FAST ALARM (range/rate/echo/tone).
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

## UI refinement 0.3.1

The [OP-1 field / Sidekick study](docs/research/te-interface-study.md) informs an
original refinement of the hardware UI: musical labels, distinct control colours,
a brief enlarged parameter view and a parameter-linked bass filter illustration.
[Preview and implementation report](docs/reports/ui-0.3.1.md). Sound bank 0.3 and
the approved organ are unchanged. The future no-Felucca implementation is deferred.

## Artwork refinement 0.3.2

Smoother native curves and more developed organ/speaker artwork, with refined
piano, brass and siren housings. [Before/after and report](docs/reports/ui-0.3.2.md).
The physical screen remains 240 × 240; this is an artwork/rendering improvement.

UI 0.3.3 gives the melodica an expressive reed-player character: its mouth opens,
cheeks puff and breath appears with the sound. See the
[native rest/play comparison](docs/images/ui-0.3.3-melodica.png).

UI 0.3.4 replaces the human melodica player with an instrument creature and adds
siren/drum personalities, a clearer guitar, and characterful effects pages.
[See eight native screens](docs/images/ui-0.3.4-characters.png).

Sound bank 0.4 refines only melodica, brass, guitar, dub siren and drums.
The approved organ, piano and bass patches are protected by forty PCM hashes.
The changes are audition candidates; see [SOUND_DESIGN.md](SOUND_DESIGN.md).

UI 0.5.1 refines SEQ with an animated 45-label header and a reggae-themed palette.
On the stopped piano roll, hold EDIT then SEQ for one second to clear the track;
hold SAVE to undo. [Details](docs/reports/ui-0.5.1.md).
UI 0.5.2 adds a record character whose mouth reacts to notes and drum voices;
its groove marker follows the loop. The same character appears in the arranger.
