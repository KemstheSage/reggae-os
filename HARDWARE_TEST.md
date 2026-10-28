# REGGAE-OS: first FM-1 hardware test

Candidate: UI 0.7.1 / sound bank 0.5, Felucca 1.6.1 base.
This is the first physical test candidate, not hardware-certified firmware.
No extra voicings, new engines, upstream rebase or bootloader changes are included.

## Before installation

1. Charge the FM-1. While it still runs stock firmware, check every key, button,
   knob, speaker and headphone output. Record the displayed stock version.
2. Connect directly to this Mac with a USB **data** cable. Close other MIDI editors
   and updaters. Leave other synths disconnected for this first test.
3. Open `build/hardware-test/CHECK DEVICE.command`. This verifies the candidate
   checksum and requests the connected device identity. **It does not flash.**
   Copy its result back into the project conversation so the installation route
   can be checked against the actual stock version. No device found usually means
   a cable, power, port or application-ownership issue; it is not a reason to force an update.
4. Keep the local vendor recovery files and a copy of any wanted stock presets.
   Stock preset export has not been performed. The bundled CLI does not back them up.

The prepared package is `build/hardware-test/reggae-os-ui-0.7.1-hwtest1.fwsc`.
Use the `.fwsc`, not the raw `.bin`. `manifest.json` and `SHA256SUMS` identify it.
Its USB/update identity remains **FM-1_900**, the existing development identity;
verify **REGGAE-OS UI 0.7.1** in Settings → SYSTEM → INFO after installation too.
An FM-1_900 identity alone does not uniquely identify this build.

## Installation route, after the identity check

The checked-in installer supports a local package. From the project directory:

```sh
.venv312/bin/python tools/fm1_install.py --info
.venv312/bin/python tools/fm1_install.py build/hardware-test/reggae-os-ui-0.7.1-hwtest1.fwsc
```

The second command is a **write operation**, not part of preparation. It displays
package/device identities and asks `Install?`. Run it only after checking the
connected stock version and agreeing to the physical test. Do not add `--yes`
or `--force`. Do not interrupt a write, unplug or let the computer sleep. The
upstream public Felucca installer installs upstream Felucca, not this candidate.
There is deliberately no double-click automatic flashing launcher in this kit.

## First ten minutes after a successful install

Start MASTER low, then raise it gradually. Prefer headphones or line out to judge
bass; also check the speaker separately. Use internal clock initially.

| Test | Action | Pass condition |
| --- | --- | --- |
| Boot | Power on without holding keys | REGGAE interface appears; no reboot loop or crash screen |
| Controls | Test all keys, octave buttons, seven encoders and MASTER | Labels/actions match; every key releases |
| Organ | T1 Roots Organ; repeated short chords; four main knobs | Clear attacks/releases, audible control changes, animation responds |
| Bass | T2 Deep Roots; low notes and fast transitions | Stable bass, no persistent note or unexpected dropout |
| Other sounds | Audition trumpet, melodica, piano, skank and sirens | Sound starts/stops; effects tails decay normally |
| Drums | T4 Roots Kit; kick/snare/hat combinations | Hits trigger consistently, including kick under chords |
| Chords | SCL → CHORD; try MAJ6/MIN9 and each voicing on a POLY organ | Expected chord; changing type while held does not leave stuck notes |
| Mixer | HOME from instrument; C5 LEVEL, D5 PAN, E5 REVERB | Knobs 1–4 control tracks 1–4; headphone pan changes sides |
| Sequencer | Record a short part on each track; PLAY/STOP | Parts remain in time; stopping ends notes appropriately |
| Editing | Stopped melodic STEP: EDIT+keys add; REC+keys replace | Changes affect only selected step; SAVE held undoes |
| Persistence | SAVE → PROJECT; use an unused A–D slot; reboot and load | Sounds, chord settings and patterns return |

Then run a 10-minute four-track loop with chords, bass and drums. Turn parameters,
visit animated screens and use held-FX echo throws. Note any clicks, missed notes,
freezes or resets and which screen/patch was active. Check SYSTEM → INFO before
and during load. These observations determine actual processing headroom.
Keep **SYSTEM → 2ND CORE OFF** for the initial baseline. If glitches occur,
compare ANIM OFF against ON and record the difference; do not assume the graphics
are the cause. Eight shared voices can legitimately steal notes with long releases.

Only after this passes: check USB audio, TRS/USB MIDI input, sustain/bend and
external MIDI clock. External-clock fixes in upstream 1.7.0 have not been backported
into this frozen candidate; long-running clock and busy CC input need particular scrutiny.

## Recovery preparation and limits

Vendor downloads: https://www.m-vave.com/download (FM-1 under PC Firmware;
M-UPGRADE under PC Software). The preparation kit stores downloaded files and
source URLs/checksums in `recovery/`. Downloading is not installation or a tested
recovery. Keep these proprietary vendor files local; do not add them to GitHub.
The vendor's listing version and the package's internal identity can differ;
`recovery-check.json` records the actual inspected bytes.

**Recovery file inspected on 28 October:** the current vendor download is
`FM-1_016` (V16), SHA-256
`d017fb65e70aca97ee29d944470c3515a74581df5c0576bf70de917c7b2b61d5`.
It is not the known V15 image accepted by this installer. The V16 return path
has not been validated; do not treat the download as a verified rollback.

Our CLI accepts the known unmodified V15 hash without forcing. A newer/unknown
stock hash must be investigated, not accepted with `--force`. Upstream 1.7.0
specifically changes return-to-stock handling, so do not assume its M-UPGRADE
instructions apply unchanged to this 1.6.1-derived build. Recovery on the physical
unit is still untested. A no-USB/black-screen situation requires a separate
assessment; do not repeatedly try random firmware packages.

If any test fails, record the screen/photo, patch, track, keys held, connected
cables and steps to reproduce. Retain the candidate and logs. Do not factory-reset
or recalibrate before recording the evidence.

### Installation update — 28 October 2026

The physical FM-1 reported stock V15. REGGAE-OS was installed successfully and
reconnected as FM-1_900. Screen, sound and control checks above remain pending.
An exact trusted V15 recovery file is now stored locally in
`build/hardware-test/recovery/FM-1_015.fwsc`, downloaded from M-VAVE’s
[official repository](https://github.com/m-vave/fm1_open/blob/main/sdk/firmware/FM-1_015.fwsc).
The installer’s simulated V15 restore test passes with this file. Physical
rollback has not been tested; the earlier V16 limitation remains applicable.
