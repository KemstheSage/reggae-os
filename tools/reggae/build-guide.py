#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Build a static musician's guide from tests/reggae_guide.c framebuffer captures.
Run with Pillow installed, after the capture harness. No browser-only UI artwork.
"""
from pathlib import Path
from html import escape as h
import json
from PIL import Image
SRC = Path('build/reggae/guide')
OUT = Path('docs/guide')
OUT.mkdir(parents=True, exist_ok=True)
# Descriptions are positional, matching the four labels captured from firmware.
# Keep this deliberately explicit: an unrecognised page fails the build.
P = {}
def page(n, purpose, knobs, how=''):
    assert len(knobs.split('|')) == 4
    P[n] = (purpose, knobs.split('|'), how)
page(0,'Shape the loudness of each note. Try a short attack and release for crisp chops.', 'Time to rise after a key press|Time to fall to the sustain level|Level while the key stays held|Time to fade after releasing the key')
page(1,'Let the note envelope move other parts of the sound.', 'Filter movement amount|Pitch movement amount|Engine-specific waveform/timbre movement amount|Tempo-synchronised envelope timing')
page(2,'Add repeating movement to the selected sound.', 'Modulation speed|Modulation waveform|Starting phase|Time for modulation to fade in')
page(3,'Choose how the LFO starts and follows the beat.', 'Free-running or tempo-synchronised timing|Note-trigger/restart behaviour|Positive-only or bipolar movement|Unused')
page(4,'Decide what the LFO moves. Start with small amounts.', 'Pitch modulation depth (vibrato)|Filter modulation depth|Engine-specific timbre modulation depth|Amplitude modulation depth (tremolo)')
page(5,'Connect a modulation source to a sound parameter.', 'Select a modulation slot|Select the source|Select the destination|Amount and direction of the modulation')
page(6,'Set the selected track’s distortion and effect sends.', 'Distortion amount|Chorus send|Delay send|Reverb send', 'Raise a send to hear its effect. The later delay, reverb and chorus pages set the shared effects for all tracks.')
page(7,'Chop a sustained sound into rhythmic pulses.', 'Slicer mode|Rhythm pattern|Chopping speed|Chopping depth')
page(8,'Process the selected track through an insert effect.', 'Choose OFF, SOFT, HARD, FOLD, FUZZ, CRUSH, PHASR, FLANG or CHOR|Drive for distortion; bit depth for CRUSH; speed for swept effects|Tone for distortion; sample rate for CRUSH; depth for swept effects|Output level for distortion; low-pass cutoff for CRUSH; feedback for swept effects', 'The labels change with the effect type. OFF leaves A/B/C inactive. Use INSERT 2 to blend the result.')
page(9,'Blend the track’s insert effect with its original sound.', 'Dry/wet mix|Unused|Unused|Unused')
page(10,'Set the shared dub delay.', 'Delay-time division|Repeat feedback|Repeat tone/colour|Delay return mix', 'Raise DLY on the track’s FX page to feed the delay. High feedback creates long tails; lower it to let the repeats die away.')
page(11,'Set the shared reverb space.', 'Room, spring or hall model|Space size/decay character|High-frequency damping|Unused', 'Raise the track’s REV send on FX, or use the mixer’s REVERB mode.')
page(12,'Set the shared chorus movement.', 'Modulation speed|Modulation depth|Unused|Unused', 'Raise CHO on the track’s FX page to hear the chorus.')
page(13,'Keep notes in a chosen musical key, or transpose a part.', 'Key/root note|Scale|Keyboard quantisation mode|Transpose in semitones', 'ROOT sets the scale’s key; it does not replace the note you play. Quantisation and transpose happen before chord generation.')
page(14,'Play a complete chord from one key.', 'Key/root for scale-based chords|Scale used by DIA3 and DIA7|Chord type; OFF returns single notes|CLOSE, OPEN, INV1, INV2 or +OCT voicing', 'Choose a polyphonic organ or piano, set CHRD to MAJ6 or MIN9, then play a key. Fixed chord types follow that key; DIA3/DIA7 build chords from the selected scale. See the chord table above.')
page(17,'Choose the analogue engine’s filter response.', 'Filter type|Cutoff: lower is darker|Resonance near the cutoff|How much cutoff follows note pitch')
page(18,'Balance the first four drum lanes.', 'Kick level|Snare level|Clap level|Closed hi-hat level')
page(19,'Balance the other four drum lanes.', 'Open hi-hat level|Tom level|Rim level|Bell level')
page(20,'Trim and split an uploaded sample into playable slices.', 'Select slice marker, or END|Move the selected marker|Select SPLIT action|Select JOIN action', 'Turn an action knob then OCT+ to apply; OCT− cancels. Editing requires a populated user sample slot. Factory sources are view-only. Keys select and audition slices.')
page(21,'Edit one of the six FM operators: an oscillator that may make sound or modulate another.', 'Select operator|Coarse frequency ratio (or fixed-frequency setting)|Fine frequency adjustment|Operator output level', 'Changing a carrier level changes loudness; changing a modulator level mainly changes brightness. The algorithm shows how operators connect.')
page(22,'Shape the selected FM operator over time.', 'Select operator|Select envelope stage|Rate for that stage|Target level for that stage', 'This is an operator envelope, separate from the whole-instrument ENV page. Higher rate means faster movement.')
page(23,'Set the selected FM operator’s tuning and touch response.', 'Select operator|Ratio or fixed-frequency mode|Operator detune|Velocity sensitivity')
page(29,'Choose how notes share voices and slide between pitches.', 'POLY, MONO, LEGATO or UNISON|Glide time|Glide behaviour|Mono note priority', 'One-key chords need POLY. All four tracks share eight voices; long four-note chords can steal voices from other parts.')
page(30,'Control voice allocation and the selected track’s placement.', 'Voice allocation policy|Unison detune|Track pan|Track mute')
page(31,'Spread unison voices across the stereo field.', 'Stereo spread|Unused|Unused|Unused')
page(32,'Arrange saved patterns into a complete four-track performance.', 'Select section, or + ADD|Select track cell|Choose pattern source A–D or MUTE; on + ADD create a section|Number of repetitions, 1–16', 'First save variations in PROJECT slots A–D. Each arrangement row chooses a source for each track, using the current sounds. PLAY here starts the arrangement and stops at its end. Up to 16 sections. A loop follows the first unmuted track’s pattern length, not necessarily one bar. Stop playback before editing and save the project again to keep the arrangement.')
page(33,'Save or recall one instrument sound.', 'Choose user slot|Select LOAD action|Select ERASE action|Select SAVE action', 'Turn the action knob, then OCT+ and follow the confirmation/name prompts; OCT− cancels. Stop to save. Loading a sound preserves the track’s scale/chord settings. Use PROJECT to save the whole session including chords.')
page(34,'Browse factory and saved sounds.', 'Choose a preset|Filter by engine|Favourite control|Choose list/category', 'PRESETS also browses. This is the broader library; HOME’s curated browser focuses on the reggae instruments.')
page(35,'Load an existing phrase into the selected track.', 'Choose phrase|Select LOAD action|Unused|Unused', 'Confirm LOAD with OCT+. This replaces the selected track’s pattern; save a project first if you want to keep it.')
page(36,'Keep all four parts, sounds, chord settings and arrangement together.', 'Choose project slot A–D|Select ERASE action|Select LOAD action|Select SAVE action', 'Turn the action knob then OCT+; follow overwrite/name prompts. OCT− cancels. Stop playback before saving. Slots A–D are also the arrangement’s pattern sources.')
page(37,'Clear or initialise parts of a project.', 'Clear selected track’s pattern|Initialise selected track’s sound|Remove arrangement section|Clear arrangement', 'Select an action and read its confirmation before OCT+. OCT− cancels. Hold SAVE to undo supported edits.')
page(38,'Turn held notes into a repeating arpeggio.', 'Arpeggio mode; OFF disables it|Note rate|Octave range|Note gate length', 'Hold a chord and press PLAY if required by the clock/transport setup. Use short gates for separated notes.')
page(39,'Add variation to the arpeggio.', 'Swing|Probability of playing a note|Latch/hold notes after key release|Note order')
page(40,'Write notes and chords into a pattern, one step at a time.', 'Choose step|Pitch of the selected note|Note duration; TIME when selecting REST/TIE|Normal, accent or slide feel', 'Stopped: hold EDIT and play keys to add notes; hold REC and play keys to replace them. Hold EDIT+REC to clear one step. Hold EDIT, then SEQ for one second to clear the entire roll. Hold SAVE to undo. Tap REC to arm step entry; PLAY starts/stops playback. Up to four notes per step and 64 steps per track.')
page(41,'Add variation to the selected sequencer step.', 'Play probability, then conditional triggers above 100%|Number of repeated hits within the step|Timing offset (requires quantisation OFF to hear)|Step velocity', 'PRESETS selects the step. Accent can override the velocity; ratchets and quantisation can override nudge. The screen shows these conditions.')
page(42,'Edit per-step locks and recorded parameter movement.', 'Select row|Select/move event’s step (where applicable)|Select parameter/type (where applicable)|Edit row value; PLAY row toggles automation', 'Labels change with the row. + ADD: choose step/type, then OCT+ to add. On a record OCT+ switches LOCK (one step) and AUTO (until changed). EDIT deletes/reset the row. QUANTIZE row K4 enables/disables timing quantisation; PLAY row OCT+ asks to clear automation. The four tracks share 128 automation records.')
page(43,'Mix all four tracks from one persistent page.', 'Track 1 level|Track 2 level|Track 3 level|Track 4 level', 'C5 selects LEVEL, D5 PAN, E5 REVERB: all four knobs then edit that property for T1–T4. Vertical faders always show volume. First four black keys toggle mute; hold F3/G3/A3/B3 to solo T1/T2/T3/T4. No held button is needed to stay here.')
page(44,'Set the shared transport timing.', 'Tempo in BPM|Global swing|Internal or external clock source|Unused', 'Use internal clock for standalone play. External clock requires a source sending MIDI clock/transport.')
E = {
'ANALOG': ('Oscillators and noise.|Waveform|Oscillator detune|Oscillator balance|Noise level', 'Filter and drive.|Filter cutoff|Resonance|Saturation drive|Filter key tracking'),
'PHASE': ('Phase-distortion oscillators.|First waveform|Second waveform|Phase-distortion amount|Envelope influence on distortion', 'Shape the oscillator combination.|Detune|Mix or ring-modulate the two lines|Sub-oscillator level|Wavefolding amount'),
'LOFI': ('Chip-style tone generation.|Chip model|Waveform|Pulse duty cycle|Bit crushing', 'Retro movement and tone.|Pitch sweep|Vibrato amount|Chip arpeggio|Tone/brightness'),
'SAMPLE': ('Sample playback setup.|Sample set|Pitch tuning|Bit depth|Loop mode', 'Sample colour.|Low-pass cutoff|Unused|Drive|Unused'),
'VOICE': ('Vowel/formant synthesis.|First vowel|Second vowel|Blend/talk between vowels|Formant shift', 'Voice texture.|Buzz amount|Breath/noise amount|Formant resonance|Random variation'),
'TRIO': ('Three-oscillator intervals.|Waveform|Second oscillator interval|Third oscillator interval|Detune', 'Oscillator combination and filter.|Combination mode|Filter cutoff|Resonance|Pulse width'),
'WHEEL': ('Tonewheel harmonic registration.|Drawbar registration|Low/sub harmonics|Middle/body harmonics|Upper harmonics', 'Organ articulation and movement.|Percussion setting|Key-click amount|Drive|Rotary mode/speed'),
'GRAIN': ('Granular playback source and timing.|Source|Position within source|Grain size|Grain density', 'Granular pitch and texture.|Pitch|Grain spread|Random variation|Tone'),
'PHYS': ('Physical-modelled resonator.|Model|Structure/inharmonicity|Brightness|Damping', 'Excitation and articulation.|Excitation/pluck position|Accent response|Bowed excitation amount|Direct exciter amount mixed into output'),
'DRUM': ('Electronic drum kit character.|Kit|Overall tuning|Tone|Decay', 'Drum transient and drive.|Snap|Accent strength|Kick character|Drive'),
'NOISE': ('Filtered noise generator.|Noise mode|Noise colour|Filter frequency|Resonance', 'Noise movement and texture.|Pitch/key tracking|Density|Slow drift|Crushing amount'),
'FM6': ('Six-operator FM macro controls.|Algorithm/routing|Feedback offset|Modulator level offset|Modulator ratio offset', 'FM response and source patch.|Modulator envelope-rate offset|Velocity-to-modulation sensitivity|Detune|FM patch slot'),
'SLICE': ('Sliced sample playback.|Sample source|Slice division|Start offset|Pitch', 'Slice articulation.|Playback mode|Reverse|Decay|Tone')}
SOUNDS = [
'Harmonic registration|Upper harmonics/brightness|Rotary mode/speed|Saturation drive',
'Harmonic registration|Upper harmonics/brightness|Rotary mode/speed|Saturation drive',
'Filter cutoff|Filter resonance|Filter envelope amount|Saturation drive',
'Filter cutoff|Filter resonance|Filter envelope amount|Saturation drive',
'Breath-style attack time|Filter cutoff/brightness|Pitch vibrato depth|Release time',
'FM modulator level/brightness|FM modulator envelope-rate offset|Chorus send|Detune',
'Filter cutoff/brightness|Decay time|Saturation drive|Release time',
'Filter cutoff/brightness|Attack time|Decay time|Release time',
'String brightness|String damping|Pluck/excitation position|Release time',
'Pitch envelope amount|LFO modulation rate|Delay send|Filter cutoff',
'FM modulator level/brightness|FM modulator envelope-rate offset|Modulator ratio offset|Track level',
'Harmonic registration|Upper harmonics/brightness|Rotary mode/speed|Delay send',
'Drum tone|Drum decay|Snare/transient snap|Drive',
'Pitch envelope amount|Amplitude decay length|Delay send|Filter cutoff',
'Pitch modulation range|Modulation rate|Delay send|Filter cutoff']
SPECIAL = {
'drum-grid': ('SEQ with a DRUM track selected', 'Program eight drum lanes in the pattern.', 'Step|Drum lane|Hit on/off|Accent on/off', 'Use PLAY to audition the loop. The grid replaces the melodic roll for the DRUM engine.'),
'mixer-pan': ('HOME → MIXER, then D5', 'Place each track left or right.', 'Track 1 pan|Track 2 pan|Track 3 pan|Track 4 pan', 'C5 returns the knobs to volume; E5 switches them to reverb sends.'),
'mixer-reverb': ('HOME → MIXER, then E5', 'Send each track to the shared reverb.', 'Track 1 reverb send|Track 2 reverb send|Track 3 reverb send|Track 4 reverb send', 'C5 returns to volume; D5 switches to pan. Set the reverb character on FX → REVERB.'),
'held-0': ('Hold EDIT outside the stopped melodic STEP page', 'Select an engine and its factory sound.', 'Engine|Preset number|Favourite control|Unused', 'On stopped STEP, hold EDIT is deliberately reserved for adding notes.'),
'held-1': ('Hold SCL', 'Set the chord keyboard without leaving your current page.', 'Key/root|Scale|Chord type|Voicing', 'Release SCL to return. The normal CHORD page offers the same four controls without holding.'),
'held-2': ('Hold REC outside the stopped melodic STEP page', 'Prepare the metronome and recording lead-in.', 'Metronome: OFF / REC / ON|Count-in length|Click level|Unused', 'Release REC to return. On stopped STEP, hold REC and keys replaces notes instead.'),
'held-3': ('Hold SEQ', 'Change pattern length and timing, or use the keyboard tools.', 'Pattern length|Step division/rate|Pattern swing|Pattern gate length', 'F4 opens Euclid. Read the on-screen key map before using clear/random tools; hold SAVE afterwards to undo.'),
'euclid': ('Hold SEQ, then F4', 'Distribute hits evenly across a cycle.', 'Number of hits|Number of steps|Rotate the rhythm|Unused on melodic tracks; drum lane on DRUM', 'OCT+ applies; OCT− cancels. The preview is not committed until applied.'),
'held-4': ('Hold FX', 'Perform momentary dub effects over the playing mix.', 'Low/high-pass filter sweep|Crushing amount|Delay throw amount|Punch-in effect depth; shimmer amount for harmonic effects', 'White keys F3/G3/A3: repeat 1/8, 1/16, 1/32; B3 reverse; C4/D4 low/high-pass; E4 tape stop; F4 freeze; G4/A4 octave up/down; B4 flanger; C5 phaser; D5 reverb throw; E5 echo throw; F5 gate; G5 mono. Release to end the punch-in; tails can continue. FX LATCH in settings changes release behaviour.'),
'held-5': ('Hold GLO (optional shortcut)', 'Adjust the four track volumes temporarily.', 'Track 1 level|Track 2 level|Track 3 level|Track 4 level', 'Use HOME → MIXER for the persistent page with pan and reverb.'),
'credits': ('Hold HOME → SYSTEM → ABOUT → OCT+', 'Read project and upstream attributions.', 'Unused|Unused|Unused|Unused', 'PRESETS scrolls; OCT− returns to settings; HOME exits.'),
'info': ('Hold HOME → SYSTEM → INFO → OCT+', 'Inspect firmware identity and runtime readings.', 'Unused|Unused|Unused|Unused', 'OCT− returns to settings. Emulator CPU readings do not establish physical FM-1 performance.'),
'name': ('SAVE → USER or PROJECT → SAVE → OCT+', 'Name the sound or project before writing it.', 'Cursor position|Character at cursor|Sound category for USER; random name for PROJECT|Unused', 'White keys type letters; repeated taps cycle a letter group. Black F# moves left, G# inserts space, A# moves right, C# deletes, D# switches letters/numbers. OCT+ writes, OCT− cancels. Maximum 12 characters; stop transport to save.'),
'confirm': ('After selecting an action that asks for confirmation', 'Check the exact operation before applying it.', 'Unused|Unused|Unused|Unused', 'OCT+ confirms; OCT− cancels. Read the action and track/slot in the message.'),
'update': ('Firmware-update shortcut: hold both OCT buttons', 'Countdown before entering the device update path.', 'Unused|Unused|Unused|Unused', 'Release the OCT buttons to cancel. This is not needed for normal music-making. See HARDWARE.md before flashing a physical FM-1; hardware installation has not been tested for REGGAE-OS.'),
'count-in': ('Start armed recording with a configured count-in', 'Shows the lead-in beats before recording starts.', 'No count-in-specific assignment|No count-in-specific assignment|No count-in-specific assignment|No count-in-specific assignment', 'PLAY stops/cancels. Set count-in length using held REC or Settings → AUDIO.'),
'calibration': ('Hold HOME → SYSTEM → CALIBRATION → OCT+', 'Assign/check the physical controls.', 'Follow the named control prompt|Follow the named control prompt|Follow the named control prompt|Follow the named control prompt', 'Only operate the control named on the screen; any piano key cancels. This is device setup, not a musical edit page.')}
SETTINGS = [
'COLOR: stored palette preference; the new REGGAE artwork keeps its own palette. STYLE: filled or outlined controls. LARGE: expanded parameter feedback while editing. ANIM: ON, OFF, or idle animation. LEDS: key-light mode. SCREEN OFF: idle timeout. SCOPE: output or pre-master mix measurement. HOME: legacy scope/tracks/levels preference; curated reggae sounds use their own performance screens.',
'HOLD: time before a button opens its held layer. KNOB ACCEL: faster turns make larger changes. FX LATCH: keep punch-in effects latched. BPM LOCK: restrict accidental SELECT tempo edits. SCALE LEDS: show notes in the scale. STEP PREVIEW: audition the selected step. CHORD ENTRY: HOLD or ADD note-entry behaviour. SELECT: choose BPM, PAGE, modulation wheel or velocity for the SELECT encoder.',
'SPEAKER EQ: FLAT, LOWCUT or BASS+; this also affects line/headphone and USB output. USB LEVEL: follow MASTER or fixed level. CLICK: OFF, recording-only or ON. CLICK LEVEL: metronome volume. COUNT-IN: OFF, one or two bars. TUNE: master tuning offset in cents, saved with the project.',
'MIDI IN: input routing mode. T1–T4 CHANNEL: channels when using TRACK routing. AUTO SELECT: incoming notes can select their track. SCREEN WAKE: panel-only or panel plus MIDI. LEARN CLEAR: OCT+ immediately removes learned CC mappings. Channel/routing choices belong to the project.',
'USB SERIAL: developer-console interface. RESTORE LAST: restore/autosave the previous session. HELP: on-screen hints. 2ND CORE: hardware-only beta option when included in the build. CALIBRATION: control setup. INFO: runtime/version readings. ABOUT: credits. Use OCT+ to open action rows; HOME closes settings.'
]

def describe(r):
    key = r['id']
    if key.startswith('sound-'):
        return ('HOME, then choose a track with ALGORITHM and sound with PRESETS', 'Play the selected instrument and use its four performance controls.', SOUNDS[int(key[-2:])].split('|'), 'Tap HOME through MIXER and CLOCK to return here. T4’s curated browser stays on drums; advanced EDIT pages can change its engine. These controls edit the sound you hear; save the project or a user sound to keep changes.')
    if key.startswith('page-'):
        n = int(key[5:7])
        family = 'ENV' if n<2 else 'LFO' if n<6 else 'FX' if n<13 else 'SCL' if n<15 else 'EDIT' if n<32 else 'GLO' if n==32 else 'SAVE' if n<38 else 'ARP' if n<40 else 'SEQ' if n<43 else 'HOME'
        reach = f'Tap {family} to enter its pages; tap again to cycle to {r["title"].split(" / ")[0]}'
        if n in (15,16):
            text, *knobs = E[r['engine']][n-15].split('|')
            return reach, text, knobs, 'These controls depend on the selected engine. Choose the engine by holding EDIT outside the stopped STEP editor. Change one control while playing to hear its effect.'
        purpose,knobs,how = P[n]
        return reach,purpose,knobs,how
    if key.startswith('settings-'):
        tab = int(key[-1])
        return ('Hold HOME, then ALGORITHM chooses the tab', 'System settings: '+r['title'].lower()+'.', ['Change the selected row’s value']*4, 'PRESETS selects the row; all four knobs edit that same selected row, not four different rows. OCT−/+ also decrease/increase values. OCT+ opens action rows. HOME exits. '+SETTINGS[tab])
    reach,purpose,knobs,how = SPECIAL[key]
    return reach,purpose,knobs.split('|'),how

rows=[json.loads(x) for x in (SRC/'pages.jsonl').read_text().splitlines()]
assert len({r['id'] for r in rows})==len(rows)
css='''*{box-sizing:border-box}html{scroll-behavior:auto}body{margin:0;background:#101917;color:#f3e9cb;font:17px/1.6 system-ui,sans-serif}main{max-width:1080px;margin:auto;padding:24px}a{color:#f5c65a}h1,h2,h3{line-height:1.2}h1{font-size:2.4rem}h2{margin-top:48px}p{max-width:78ch}.intro,article{background:#1d2d28;border:1px solid #486454;border-radius:12px;padding:20px;margin:18px 0}nav{display:flex;gap:12px;flex-wrap:wrap}input{width:100%;padding:14px;font:inherit;background:#f3e9cb;border:0;border-radius:6px;color:#101917}.screen{width:240px;max-width:100%;height:auto;image-rendering:pixelated;border:2px solid #486454;border-radius:4px}.body{display:grid;grid-template-columns:240px 1fr;gap:24px}.reach{color:#f5c65a}table{width:100%;border-collapse:collapse;font-size:.95rem}td,th{text-align:left;padding:9px 6px;border-bottom:1px solid #486454;vertical-align:top}td:first-child{min-width:85px}small{color:#bdd0bd}.toc{columns:3;column-gap:24px}.toc a{display:block;font-size:.95rem;break-inside:avoid}.hidden{display:none}button{padding:10px;font:inherit;background:#f5c65a;border:0;border-radius:5px}@media(max-width:650px){main{padding:14px}.body{grid-template-columns:1fr}.screen{margin:auto;display:block}.toc{columns:2}h1{font-size:2rem}article{padding:14px}}@media print{input,button,nav{display:none}body{background:white;color:black}article,.intro{background:white;border:1px solid #aaa}a,.reach,small{color:black}article{break-inside:avoid}.body{grid-template-columns:180px 1fr}.screen{width:180px}}'''
chords=[('SUS2','C D G','Open suspended sound'),('MAJ6','C E G A','Warm major colour'),('MIN6','C E♭ G A','Minor with a raised sixth'),('ADD9','C E G D↑','Major triad plus ninth'),('mADD9','C E♭ G D↑','Minor triad plus ninth'),('DOM9','C E B♭ D↑','Dominant ninth; fifth omitted'),('MAJ9','C E B D↑','Major ninth; fifth omitted'),('MIN9','C E♭ B♭ D↑','Minor ninth; fifth omitted'),('7SUS4','C F G B♭','Suspended dominant seventh'),('DIM','C E♭ G♭','Diminished triad'),('DIM7','C E♭ G♭ B𝄫','Diminished seventh; B𝄫 sounds as A'),('m7b5','C E♭ G♭ B♭','Half-diminished seventh'),('AUG','C E G♯','Augmented triad'),('mMAJ7','C E♭ G B','Minor-major seventh'),('6/9','C E A D↑','Major sixth/ninth; fifth omitted')]
parts=['<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>REGGAE-OS — screenshot quick guide</title><style>'+css+'</style><main id="top"><nav><a href="../">Open emulator</a><a href="#chords">New chords</a><a href="#pages">Find a page</a></nav><h1>REGGAE-OS<br>Screenshot quick guide</h1><p>UI 0.7.1 · Sound bank 0.5 · Screens captured from the shared firmware at the FM-1’s native 240 × 240 resolution. This guide is readable on a phone and can be printed from your browser. Physical FM-1 operation and iPhone Safari have not been verified on devices.</p><section class="intro"><h2 style="margin-top:0">Start here</h2><ol><li>Open the emulator and tap its audio-start control. Play its virtual keys.</li><li>On the instrument screen, ALGORITHM chooses track T1–T4; PRESETS chooses a sound. The four lower knobs match the four labels on the screen.</li><li>Tap a section button (ENV, LFO, FX, SCL, EDIT, SAVE, ARP or SEQ); further separate taps cycle that section’s pages. Avoid rapid double taps when navigating.</li><li>Tap HOME to get back to the instrument area. From instrument play, HOME cycles MIXER → CLOCK → instrument. Hold HOME for settings.</li><li>MASTER controls output level. SELECT’s job is configurable in Settings → CONTROL; do not assume it always selects a page.</li></ol><p>“K1–K4” means the four lower parameter knobs, from left to right. Turn an action knob to select an operation, then use OCT+ to confirm and OCT− to cancel when prompted. Settings use all four knobs to edit the same selected row. Blank knobs are intentionally unused.</p><p><strong>Record a first loop:</strong> from instrument play, tap REC and play: the first note starts recording. Tap REC to finish the overdub. Select another track and repeat. PLAY stops. On the stopped STEP editor, REC instead arms step entry. Save the complete session through SAVE → PROJECT.</p></section><section id="chords"><h2>15 new chord types</h2><p>Tap SCL to reach CHORD GARDEN. K3 chooses the chord, K4 the voicing. Try MAJ6, MIN7 or MIN9 on organ/piano; they are useful starting colours, not substitutes for offbeat articulation. Existing OFF, DIA3, DIA7, MAJ, MIN, DOM7, MAJ7, MIN7, SUS4 and POW remain available.</p><table><tr><th>Type</th><th>Example on C</th><th>Meaning</th></tr>']
parts += ['<tr>'+''.join('<td>'+h(v)+'</td>' for v in c)+'</tr>' for c in chords]
parts += ['</table><p>↑ means the next octave. CLOSE uses the listed pitches. OPEN raises the second tone an octave; INV1/INV2 raise the first one/two tones. +OCT adds a lower root: when already using four tones it drops a tone to stay within four voices. Extended ninth/6/9 shapes keep their colour tones by replacing the upper root. At the ends of the keyboard, out-of-range tones are omitted.</p><p>DIA3/DIA7 follow ROOT and SCL; fixed shapes follow the note you play after keyboard quantisation/transposition. Choose POLY under EDIT → VOICE to hear all chord notes. MONO/LEGATO/UNISON play only the root; drum kits ignore chord mode. Eight voices are shared across the four tracks. PROJECT saves the chord settings; loading a user sound deliberately keeps the current track’s chord settings.</p></section><h2 id="pages">Find a page</h2><label for="search">Search by page, engine, control or action</label><input id="search" type="search" placeholder="Try chord, mixer, cutoff, save, drum…"><p id="count"></p><div class="toc">']
parts += [f'<a href="#{r["id"]}">{h(r["title"])}</a>' for r in rows]
parts += ['</div>']
for r in rows:
    reach,purpose,knobs,how=describe(r)
    assert len(knobs)==4, r['id']
    im=Image.open(SRC/(r['id']+'.ppm'));assert im.size==(240,240)
    im.save(OUT/(r['id']+'.png'),optimize=True)
    parts += [f'<article id="{r["id"]}" data-page><h2 style="margin-top:0">{h(r["title"])}</h2><p class="reach">{h(reach)}</p><div class="body"><img class="screen" loading="lazy" width="240" height="240" src="{r["id"]}.png" alt="Actual firmware screen: {h(r["title"])}"><div><p>{h(purpose)}</p><table><tr><th>Knob / label</th><th>What it does</th></tr>']
    for i,(label,meaning) in enumerate(zip(r['knobs'],knobs),1):
        parts += [f'<tr><td>K{i} · {h(label or "—")}</td><td>{h(meaning)}</td></tr>']
    parts += ['</table></div></div>',f'<p>{h(how)}</p>' if how else '', '<small><a href="#pages">Back to page index</a></small></article>']
parts += ['<p>Guide generated from firmware captures by tests/reggae_guide.c and tools/reggae/build-guide.py. Settings screenshots show the first visible rows; PRESETS scrolls to the remaining rows described alongside them. Crash diagnostics and bootloader internals are not musical pages and are outside this guide.</p><script>const input=document.querySelector("#search"), pages=[...document.querySelectorAll("[data-page]")];function filter(){const q=input.value.toLowerCase().trim();let n=0;for(const p of pages){const show=p.textContent.toLowerCase().includes(q);p.classList.toggle("hidden",!show);if(show)n++;}document.querySelector("#count").textContent=n+" of "+pages.length+" screen references";}input.addEventListener("input",filter);document.querySelectorAll(".toc a").forEach(a=>a.addEventListener("click",()=>{input.value="";filter();}));filter();</script></main></html>']
(OUT/'index.html').write_text('\n'.join(parts))
(OUT/'pages.json').write_text(json.dumps(rows,indent=2)+'\n')
print(f'Built {len(rows)} screenshot references at {OUT}/index.html')
