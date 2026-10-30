# UI 0.7.6 TEST — direct pressure filters, original FX map

The physical trial of 0.7.5 reported approximately 800 ADC counts with a light
press and 420 with a firm press. The user confirmed CALIBRATED/LIVE and normal
release, but the effect felt binary. These are user observations, not automated
verification of pressure response across the keyboard.

Two problems in the implementation/workflow are addressed:

1. LIVE temporarily replaced the first four FX assignments with echo/reverb/filter
   shortcuts. This removed the original repeat/reverse shortcuts from those keys.
   0.7.6 never overrides the saved map. All sixteen original effects remain in
   place; deliberate user reassignments still work in every pressure mode.
2. Calibrated filter depth multiplied the old one-bar automatic sweep. It now
   directly sets LPF/HPF cutoff, independent of tempo and how long the key has
   been held. Light means open LPF; firm means low cutoff; easing opens it again.
   The direct path uses a roughly 49 ms full-range slew at 128-frame blocks,
   plus the existing raw-sample smoothing. Digital fallback keeps the old sweep.

This fixes a demonstrated filter-control mismatch; it does not establish the
cause of the earlier perceived binary echo. Calibration is per physical key;
upper seven keys cannot be sensed by the current reader. FX LATCH stays binary.
The PUNCH-IN header now shows a pressure percentage, CALIBRATE FIRST, DIGITAL
ONLY, SIGNAL LOST or LATCH BYPASSED instead of silently concealing these cases.
A global filter macro can still dominate the key; use FILTER OFF for the trial.

## Physical retest

Start in KEY PRESSURE / TEST. On the default FX map, LPF is **C4**, the fifth
white key from the left (**physical key 8**, counting black keys too).
Calibrate this key with K3 LIGHT and K4 FIRM while holding it. Release it, set
LIVE and return to PUNCH-IN. Keep FILTER OFF and FX LATCH OFF. Hold C4 lightly,
then press harder and ease off. The displayed percentage and the treble cut
should change together. Releasing must end the effect. Calibration is RAM-only
and is cleared by the update/restart, so the earlier lowest-key capture must be
redone on C4. The lowest key is once again REPEAT 1/8, not ECHO.

## Validation

- `reggae_pressure_filter.c`: synthetic raw 800/610/420 on physical-key index 7
  through key mapping, normalization and real LPF DSP. Measures decreasing
  5.5 kHz spectral energy in PCM, immediate full-cutoff target independent of
  tempo, easing without release, release cleanup, original digital sweep and HPF.
- `reggae_pressure_input.c`: calibration, stale data, continuous-held effects,
  UI navigation, explicit reassignment preservation and all sixteen default keys
  unchanged in LIVE; captures the real 240×240 pressure meter screen.
- Existing DSP, project-isolation and punch-page regressions retained.

Hardware build and WebAssembly logs and checksums belong to the frozen candidate
kit. Revised response is HARDWARE UNTESTED until the user tries it. Physical raw
ADC observations alone do not validate the musical response or all keys.
