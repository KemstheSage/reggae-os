# FM-1 hardware / compatibility

REGGAE-OS targets the M-VAVE FM-1 through Felucca 1.6.1, commit `a53ecf4`.
These specifications are **CONFIRMED in source**, not independently measured on a device.
No physical FM-1 has been used for this project.

| Area | Architecture and source |
| --- | --- |
| CPU | JieLi AC79/WL82 family, pi32v2 target; `firmware/hal`, `tools/build.py` |
| Display | ST7789-class, 240 × 240, RGB565 big endian, SPI1 at configured 12 MHz; `src/lcd.c` |
| Rendering | Existing 240 × 124 strip buffer, 59,520 bytes, reused for every section; `src/gfx.c`. No new framebuffer on hardware. |
| Display throughput | A full 115,200-byte frame alone takes at least 76.8 ms at 12 MHz. REGGAE-OS throttles to 10 Hz; the current prototype redraws all three strips. Actual scheduling/audio contention requires hardware measurement. |
| Memory map | `firmware/app.ld`: 72 KiB data/BSS, 360 KiB pool, 24 KiB RAM code region, retained region and guarded stacks within the SRAM map. These are linker allocations, not additional memory chips. |
| Flash | Existing 1 MiB NOR layout and 0x8DFBC-byte XIP application region; retain all partition boundaries. |
| Audio | Fixed-point DSP, 44.1 kHz, 32-sample control blocks, 128-frame I2S DMA halves. Eight voices shared across four parts. |
| MIDI | Existing USB/TRS input, bend, sustain, channels, program selection, clock and USB audio paths retained. Browser does not emulate physical USB/TRS hardware. |
| Boot/update | Existing loader, package checks and boot guards unchanged. No flashing performed. |

## Actual controls

`src/panel.c` defines 14 buttons: FX, SCL, ENV, LFO, EDIT, GLO, HOME,
SAVE, ARP, SEQ, PLAY, REC, OCT−, OCT+. Seven encoders: SELECT, ALGORITHM,
PRESETS and KNOB 1–4. MASTER is a separate ADC potentiometer.
The keyboard has 27 keys (F3–G5 in the emulator before octave shifts).
The default matrix mappings and hardware calibration are retained; upstream notes
some label mapping as a best guess and PLAY/REC as hardware measured.
Do not treat every mapping as independently verified.

HOME opens the performance view. PRESETS cycles the thirteen curated sounds when
already on one of them on HOME; other pages retain full-library browsing.
EDIT, ENV, FX, held HOME menu and SAVE retain upstream navigation.
Select lower octaves for bass. Original MIDI channels and four-part operation remain.

## Installation and recovery (future physical testing)

Read the upstream [installation instructions](https://github.com/hugelton/Felucca/wiki/Installation)
and [FM-1 transporter](https://github.com/kurogedelic/FM-1-transporter) before attempting a flash.
The package is an application plus update loader and verified AC79 SDK packaging files.
Upstream's CLI accepts a local `.fwsc` (`tools/fm1_install.py`); the public Felucca
installer installs upstream Felucca, not this development build. Retain the official
vendor firmware locally for recovery; it is not redistributed here. A failed boot
may require transporter recovery. Physical installation/recovery remains **HARDWARE UNTESTED**.

## Bank 0.3 target build

COMPILES: FM-1_900 package, application 517,516 bytes; data + BSS 61,000 / 73,728
bytes; pool 328,896 / 368,640 bytes. The expanded bank uses existing engines and
buffers. No additional audio-ISR graphics work or full-screen framebuffer.
HARDWARE UNTESTED: DAC output, real-time audio headroom during 10 Hz screen updates,
physical controls, flashing and recovery remain unverified on a physical FM-1.

UI 0.3.1 final target build: application 518,524 B; data+BSS 61,032 / 73,728 B;
pool 328,896 / 368,640 B. COMPILES; physical hardware remains untested.

UI 0.3.2 target build: application 519,948 B; data+BSS 61,032 / 73,728 B;
pool 328,896 / 368,640 B. COMPILES. Integer edge smoothing adds rendering work;
physical audio/display timing remains HARDWARE UNTESTED.
