# Pressure-responsive punch-in — research and DSP prototype

Status: synthetic pressure tested against shared firmware DSP; physical sensing
NOT integrated or enabled. Public UI 0.7.4 and the installed FM-1 are unchanged.
This is continuous held-key force, not just initial note-on velocity. It should
shape an effect while a finger squeezes and eases off.

## Evidence and limits

Research source: Charles Vestal's GPL-3.0-only Velo,
https://github.com/charlesvestal/fm1-velocity at
`8f62ce0d968c91ce8bfdafbb908f81d15351650a` (README, firmware/hal/fm1_input.h,
firmware/hal/fm1_adc.h, firmware/src/app/sense.c, notes/01-lab.md).
The local research checkout is ignored under build/research/fm1-velocity.
No upstream sensing code has been copied into REGGAE-OS in this prototype.

Keys have carbon contacts, not conventional dedicated aftertouch sensors.
PA7/PA8 can be read through SARADC for 20/27 keys (F3–C5). Simple pull-up readings
saturate at a firm press; Velo obtains a wider range by briefly driving a row
through a weak GPIO driver, restoring it before digital scanning. The seven upper
keys use an experimental PA10 LED-line probe in the latest Velo, not the direct
ADC method. README reports per-key calibration is built but not yet satisfactory
and aftertouch in driven mode was not yet evaluated by feel. These are evidence
from another instrument, not verification on this user's FM-1.

REGGAE-OS currently reads MASTER and battery through that same ADC in its main
loop. Copying Velo's tick hook alone would race those reads. A future reader must
coordinate ADC ownership, restore GPIO modes before scanning, preserve LED
refresh, time out safely, and retain a way back to digital keys. Do not enable
GPIO-drive/probe experiments in the general release without a controlled hardware
session and calibrated readings.

## Shared-DSP prototype

perform.c accepts a normalized Q15 amount (0..32768) per effect through
perf_pressure_set. The caller must run in the audio/key-input context, aggregate
multiple held keys assigned to one effect, and mark unavailable/stale readings
invalid. No actual physical/emulator input calls this yet. Release invalidates
an effect's previous pressure; missing sensing returns the existing button depth.

Pressure scales filter sweep depth, echo/reverb throw sends, buffered effect wet
mix, flanger/phaser depth, gate depth and mono blend. Existing ramps smooth the
changes. Knob-driven filter macros retain their normal precedence. Track mutes
and latched effects stay binary, avoiding a latched effect inheriting an abandoned
finger-pressure value. No instrument sound or sequencer velocity mapping changes.

The current default D5/E5/F5/G5 effect keys are outside the direct 20-key range.
For an initial physical trial, map echo/reverb throws and filter sweeps to the
lower direct-ADC white keys. Keep repeats' rhythmic divisions and mute actions
stable; pressure changes mix/depth, not which beat division is selected. Upper-key
probing needs separate validation before promising pressure control on all keys.

## Verification

`tests/reggae_pressure.c` supplies synthetic normalized force to real DSP. It
checks increasing mono blend on actual stereo PCM, echo send rise/fall, effect
isolation under simultaneous keys, clamping, release cleanup, and identical
full-pressure versus unavailable-sensor mono output. Runs with AddressSanitizer.
The existing native performance test covers physical-control navigation and
playable chord audio with the prototype present.

Next hardware step: a separate opt-in diagnostic reader that reports raw readings,
checks MASTER/battery/LED coexistence, and measures several light/firm holds per
key on the actual FM-1 before connecting sensing to this DSP input. A browser can
test the mapping but cannot validate silicone contacts or GPIO behaviour.

Hardware-target compilation also passes: app 545,628 bytes, packaged image
610,090 bytes. This confirms compatibility of the DSP prototype only: no physical
pressure reader is included, no pressure feature is enabled, and nothing was flashed.
The previously published 0.7.4 release remains the user-facing build.
