# Pressure-responsive punch-in — opt-in hardware candidate

Status: UI 0.7.5 TEST integrates a physical ADC reader, calibration page and shared
DSP mapping. Physical sensing remains **HARDWARE UNTESTED**. Public UI 0.7.4 and
the installed FM-1 are unchanged by preparing this candidate. This is continuous
held-key contact sensing, not note-on velocity or a dedicated aftertouch sensor.

## Evidence and attribution

The ADC and weak-row-drive technique is adapted from Charles Vestal's
[fm1-velocity](https://github.com/charlesvestal/fm1-velocity), GPL-3.0-only,
commit `8f62ce0d968c91ce8bfdafbb908f81d15351650a` (README, fm1_input.h,
fm1_adc.h, sense.c, notes/01-lab.md). The new HAL carries attribution.

Carbon-contact resistance can be read via PA7/PA8 SARADC for the lower twenty
physical keys (F3–C5 in the untransposed keyboard). Ordinary pull-ups give little
range; Velo briefly drives a row to widen it. Its readings and calibration reports
come from another device; they do not prove sensitivity or reliability on ours.
The upper seven keys require a separate experimental LED-line probe; we have
not implemented that probe. They remain digital.

## Implementation

`firmware/hal/fm1_pressure.h` samples one selected row between column latches,
using only the weakest HD0=0/HD=0 drive. The next tick takes or times out the
conversion and restores the exact saved GPIO state **before** the digital read.
No waiting, allocation or graphics in this hook. OFF drains the pending conversion
and releases the ADC at the next tick. The normal debounce/LED pipeline remains.

ADC ownership is coordinated with the main loop. OFF uses the original polled
MASTER/battery path. TEST/LIVE reserve the ADC for timer scheduling, alternate
PA7/PA8 frames, and use PA8's two non-note slots for MASTER/battery conversions.
Main-loop readers use these cached values. Electrical settling, ISR overhead,
volume/battery accuracy and LED coexistence still require physical measurement.

`pressure.c` smooths raw ten-bit readings, batches 32 samples, and accepts light
and firm captures only while a supported key is held with recent readings and
batch spread <=60 counts. Light must exceed firm by at least 32 ADC counts.
These initial thresholds need evaluation on real contacts. Calibration is per key,
RAM-only and does not modify projects or settings storage. Sensing boots OFF.

LIVE normalizes calibrated keys to Q15 0..32768, continuously refreshed in the
audio keyboard block. Missing/stale (>40 ms) readings on a calibrated key yield
zero depth; uncalibrated/unsupported keys retain normal full-depth digital FX.
Multiple keys on one effect use the greatest depth, with an unsupported key
providing full depth. Release clears pressure ownership. Latch and track mutes
stay binary. Regular notes, MIDI, note velocities and instrument patches are unchanged.

Pressure scales filter sweep depth, echo/reverb sends, buffered-effect wet mix,
flanger/phaser depth, gate and mono blend through existing effect ramps. Knob
filter macros retain precedence. In LIVE only, the first four white keys map to
ECHO, REVERB, LPF, HPF. This temporary map is shown on PUNCH-IN and cannot be
reassigned while LIVE. OFF/TEST restore the saved FX key map.

## Test procedure

See [pressure test instructions](../reports/pressure-hardware-test.md). Tests feed
synthetic readings into actual shared firmware; register-model tests verify
ownership/restoration but cannot validate electrical operation. A browser does
not acquire pressure from the FM-1's contacts. No physical test success is claimed.
