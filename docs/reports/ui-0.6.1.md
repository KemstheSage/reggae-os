# UI 0.6.1 — persistent four-track mixer

Tap **HOME from the instrument screen** to open MIXER. It remains open with
no held button or double-tap. Knobs 1–4 control levels T1–T4; the first four
black keys toggle mutes and F3/G3/A3/B3 solo while those keys are held.
HOME cycles MIXER → TRACK MIX → CLOCK → instrument. TRACK MIX retains the
selected track's level, pan, reverb send and mute. GLO hold remains optional.

![Persistent mixer, native 240×240](../images/ui-0.6.1/mixer.png)

The mixer uses the same state-responsive speaker characters as UI 0.6.0.
Jiant's influence so far is behavioural: muted/unsoloed faces sleep, active
voices open mouths, faders show level, and chord faces represent actual chord
notes. The rest of the instrument artwork is unchanged by these two releases;
this is not a complete Jiant-style redesign. No Jiant artwork or code was copied.

Changed shared firmware: new menu graph/page in params.c, shared rendering in
reggae_seq_ui.c, four-track editing/momentary control in ui_input.c, and normal
menu key ownership in ui_layer.c. The old selected-track mixer remains intact.

Validation: native AddressSanitizer test enters with a HOME tap, leaves all
buttons released for two seconds, turns all four knobs, toggles mute, solos,
leaves while a solo key is held, checks solo clearance, edits TRACK MIX pan,
and returns to normal note keys. Existing chord/FX portions also pass. WASM
sequencer-menu and emulator regression pass. Screenshot is the actual firmware
framebuffer. DSP and curated presets are unchanged.

FM-1 image 568,316 / 581,564 bytes; static RAM 61,064 / 73,728 bytes;
pool 328,896 / 368,640 bytes. WASM 868,015 bytes. Hardware packaging and all
memory guards pass. Physical FM-1 and iPhone Safari remain untested.
