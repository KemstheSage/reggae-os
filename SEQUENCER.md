# Making a song in REGGAE-OS

UI 0.5.8 gives STEP, DETAIL, AUTOMATION, ARRANGE, held-SEQ tools and the
Euclid preview a shared tabletop-machine layout. SAVE > TOOLS uses it too.
Compact footer hints fit inside the case. Mixer, project browser, recording
settings and confirmation dialogs retain their existing layouts.

## Play and layer four parts

UI 0.5.0 starts a fresh session with T1 Roots Organ, T2 Deep Roots Bass,
T3 Roots Trumpet and T4 Roots Kit. Previously saved projects retain their sounds.
Use **ALGORITHM** to select a track. On HOME, **PRESETS** chooses the sound;
T4 stays on the Roots Kit in this performance browser.

1. Press **HOME**, choose your track and sound.
2. Tap **REC**. The screen says READY and waits for your first note.
3. Play: your first note starts the loop at the current tempo and is recorded.
4. Tap **REC** to finish recording; your loop keeps playing.
5. Choose another track with **ALGORITHM**, tap **REC**, and play the next part.
6. Press **PLAY** to stop. Live recording is disarmed too.

REC again cancels a waiting arm. Changing tracks ends the current overdub, so
press REC on the new track before playing its part. PLAY can start a waiting
recording without a note (and uses any configured count-in). External MIDI clock
still controls transport when selected; local notes cannot start it.

This first integration uses the existing tempo and pattern length; it does not
yet infer them from a free performance. Change length/rate with the existing SEQ
tools. All four tracks share eight sounding voices: keep chords compact,
especially with long releases. T4 remains editable through advanced pages; the
new drum role is enforced in HOME's curated sound browser, not on project loading.

Tap **SEQ** for NOTE PATTERN or RIDDIM GRID. The instrument name and track number
appear above the editor. The four knobs are labelled below it:

- Notes: step, pitch, note length/time, accent/slide.
- Drums: step, drum lane, hit on/off, accent on/off.

### Edit the selected note step (transport stopped)

- **Hold EDIT + play keys:** add notes to the selected step. Existing notes stay;
  repeated pitches are not duplicated. Up to four notes fit in one step.
- **Hold REC + play keys:** replace the notes. Replacement begins with the first
  played key, so merely holding REC does not erase anything. Notes played during
  the same hold build the replacement chord.
- **Hold EDIT + REC together:** clear the selected step and its parameter locks.
- **Hold SAVE:** undo the edit using the existing undo system.

These gestures stay on the selected step. Turn STEP to choose the next one.
EDIT alone no longer clears a note step. A quick REC tap still arms/disarms
recording. The shortcuts use the instrument's physical/emulated keyboard, not
incoming MIDI notes. They apply to the stopped melodic STEP page; the drum grid,
DETAIL page, playing/live-recording behavior and other pages retain existing
controls. EDIT/REC quick-setting layers remain available outside this context.

On the melodic STEP page, tap **REC** to arm step entry. With transport stopped, play notes/chords
into the step-entry cursor; with transport running, record live. Tap REC again
to disarm. PLAY starts/stops patterns. Hold SEQ for existing pattern tools,
including length/rate settings. Patterns support up to 64 steps; the editor
shows sixteen at a time. Ties, chords and parameter locks retain their behavior.
Tap SEQ again for DETAIL and AUTOMATION pages. HOME returns to instrument play.

## Save variations before arranging

Tap SAVE through its pages until PROJECT appears. Select A–D with knob 1;
use the labelled SAVE action (knob 4 / OCT+ confirmation, following any overwrite
prompt). Stop playback before saving. Each project contains all four parts.
Make a variation and save it in another slot. Existing data is not automatically
overwritten by this update.

The arrangement reads each track's pattern from those four saved projects, but
uses the **current instrument sounds**. It does not load a new sound at every
section. After making an arrangement, save the current project again so its
section order is retained as well.

## Arrange a whole performance

Tap **GLO** to open ARRANGE:

| Knob | Action |
| --- | --- |
| 1 SECTION | Select a section or the final + ADD row |
| 2 TRACK | Choose the track cell to edit |
| 3 PATTERN | Choose A–D or MUTE; on + ADD, create a section |
| 4 LOOPS | Repeat the section 1–16 times |

Each row shows all four track choices and its loop count. A dash means that track
is silent in that section. The selected cell is cream; missing saved sources
show a warning. The screen shows four sections at a time, scrolls through all
sixteen, and follows the playing section during song playback.

For example: use a sparse intro, bring in the bass, introduce organ/trumpet,
then mute selected tracks for a dub passage. Any track can take its pattern from
a different A–D source in the same section. PLAY on ARRANGE starts the entire
song and stops at its end. It refuses missing sources rather than substituting
a different pattern. Editing the arrangement is blocked while it plays.

A section's loop duration follows its first non-muted track; if every track is
muted it uses a 4/4 bar. Different track lengths can therefore create polymeters:
start with equal lengths when learning the workflow. The loop number is not
necessarily a bar count. Sources and automation are prepared before playback;
section changes do not read flash inside the audio callback.

## Current limits

This supports complete four-part pattern arrangements, not multitrack audio
recording or unlimited song storage. Four source projects, sixteen sections,
eight shared voices and existing automation limits still apply. Sounds stay
fixed per track across sections. No new effects, audio engines or preset changes
are included in UI 0.4.0. Recording/mixing/automation screens and a more guided
project workflow remain the next design work. Physical FM-1 and iPhone Safari
interaction have not been tested on devices.

### Clear the whole piano roll

With playback stopped on the melodic STEP page, **hold EDIT, then hold SEQ for
one second**. The screen says `HOLD 1 SEC: CLEAR ROLL`; releasing early cancels.
It clears all 64 note steps and their parameter locks on the selected track,
including steps outside the current pattern length. It keeps the sound, tempo,
pattern length and other tracks. **Hold SAVE to undo.**

EDIT + REC still clears only the selected step. The whole-roll shortcut is not
active in the drum grid, DETAIL, arrangement or during playback.

## Live controls (UI 0.6.1)

- **Tap HOME from the instrument screen:** opens the persistent four-track
  MIXER page. Knobs 1–4 control levels T1–T4. First four black keys toggle mute;
  white F3/G3/A3/B3 solo tracks while those keys are held. No button needs to be
  held to keep the page open. HOME cycles MIXER → TRACK MIX → CLOCK → instrument.
  TRACK MIX retains selected-track level, pan, reverb send and mute. Hold-GLO
  remains an optional quick-access shortcut.
- **Hold FX:** punch-in pads. White F3/G3/A3 repeat 1/8, 1/16, 1/32; B3 reverse;
  C4/D4 low/high-pass; E4 tape stop; F4 freeze; G4/A4 octave up/down; B4 flanger;
  C5 phaser; D5 reverb throw; E5 echo throw; F5 gate 1/16; G5 mono. Release to
  return to dry (effect tails can continue). Black keys 1–4 momentarily mute tracks.
- **Tap SCL twice, with separate taps:** CHORD GARDEN. Knobs: root, scale, chord,
  voicing. Choose MAJ/MIN scale, then DIA3/DIA7 for one-key diatonic chords.
  CHRD OFF returns normal notes. Eight voices are shared by all four tracks.
- Optional **Load CC0 percussion on selected track** below the emulator installs
  into an empty sample slot. F3 rim, G3 conga, A3 claves, B3 shaker, C4 tambourine.
  Hold for natural tails; use the melodic piano roll. This changes the selected
  track's sound. HOME/PRESETS returns to the curated instruments. Save through
  the usual preset/project controls. Source/credits: `percussion/ATTRIBUTION.md`.
