# REGGAE-OS 0.7.6 TEST: pressure candidate

This candidate is for a supervised physical trial. It boots with sensing **OFF**.
No firmware has been flashed merely by creating this build. Keep a backup of
any valued projects before installing; project-format changes are not part of
this experiment. Public 0.7.4 remains the stable browser release.

## Finding and using the page

Tap FX through to PUNCH-IN, then tap FX once more for **KEY PRESSURE**.
The page stays open. Its keys select the physical key to inspect and play no notes.
Numbers 1–27 count upward from the leftmost key, including black keys.

- K1 MODE: OFF / TEST / LIVE. Release keys before selecting TEST or LIVE.
  OFF is available even while keys are held.
- K2 KEY: select a physical key (or press that key).
- K3 LIGHT: while holding lightly and steadily, turn one detent to capture.
- K4 FIRM: while holding firmly and steadily, turn one detent to capture.

First choose TEST. Hold the lowest key and watch RAW ADC. A firmer press should
reduce the reading. Check that readings respond repeatedly and return sensibly
when re-pressed. The red number at the right of the status row is the cumulative
ADC timeout count; increasing counts mean the trial needs investigation.

Hold lightly for at least half a second; turn K3. Hold firmly for half a second;
turn K4. CALIBRATED means the software accepted the range, not that the sensor
has been independently verified. HOLD KEY STEADY means no recent stable batch;
NEED MORE RANGE means light/firm are reversed or less than 32 counts apart.
Repeat captures if needed. Do not use excessive force to obtain a range.

For the first filter trial, calibrate **C4**, the fifth white key from the left
(physical key 8 when black keys are counted). Release it, select LIVE and return
to PUNCH-IN. Keep the FILTER macro OFF and FX LATCH OFF. Hold C4 lightly, press
harder, then ease off without releasing. The pressure percentage should track
the change, with progressively less treble as pressure increases.

The original FX map is preserved in every mode. The lowest key is REPEAT 1/8;
C4 is LPF and D4 is HPF. Echo/reverb retain their upper-key assignments, which
are outside the twenty-key sensing range. Explicit user assignments still work.
Calibrate each supported physical key separately. The header identifies missing
calibration, unsupported keys, stale data and latch bypass. Uncalibrated keys
retain digital behaviour. Calibration is discarded on power-off/update.

Release must end the effect (existing echo/reverb tails may ring out). Track
mutes and latched effects remain binary. See [0.7.6 changes](ui-0.7.6-pressure.md).

## Checks on the real FM-1

Compare OFF with TEST/LIVE: every key must trigger and release normally outside
the diagnostic page; all buttons, LEDs and the master-volume knob must work;
battery indication should remain plausible; sustained chords and playback should
not crackle. Test several light/firm holds and simultaneous keys, then switch
OFF and confirm the original FX mapping returns. Power cycling returns to OFF
and discards calibration. This trial does not alter the sequencer's note velocity.

If keys stick, readings are erratic, LEDs misbehave or audio glitches, disable
sensing and report the mode, key and raw values. Do not treat intermittent pressure
as a supported performance feature. Returning to the frozen 0.7.4 build removes
this experiment; use the existing MIDI installer and recovery instructions.

## Evidence and limitations

The candidate includes native shared-DSP tests, simulated raw-sample/UI tests,
and a mocked-register test for twenty-key coverage, ADC ownership, timeout,
MASTER/battery slots and GPIO restoration. The firmware-rendered screenshot is
240 × 240. These tests cannot establish electrical safety, pressure calibration
quality or usable feel on a physical instrument.

Final build sizes, checksums and validation logs are in the frozen candidate's
manifest. **COMPILES** is not **HARDWARE TESTED**. No browser pressure simulation
is presented as physical sensing, and no public emulator is replaced by this trial.
