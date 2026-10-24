# Making a song in REGGAE-OS

UI 0.4.1 simplifies STEP into sixteen large pads and a single drum lane,
alongside the SONG arrangement. DETAIL, automation, mixer, saving and the
held-button tool layers retain their existing layouts for now.

## Start with four parts

Use **ALGORITHM** to select track 1–4. On HOME, choose each sound with PRESETS.
A useful starting allocation is drums, bass, organ and trumpet/melodica/siren.
All tracks share eight sounding voices: keep chords compact, especially with long
releases. There are four instrument tracks, not unlimited audio tracks.

Tap **SEQ** for MAKE A PATTERN or MAKE A BEAT. The instrument (or selected drum)
and track number appear above the pads. The four knobs are labelled below it:

- Notes: step, pitch, note length/time, accent/slide.
- Drums: step, drum lane, hit on/off, accent on/off.

Tap **REC** to arm the selected part. With transport stopped, play notes/chords
into the step-entry cursor; with transport running, record live. Tap REC again
to disarm. PLAY starts/stops patterns. Hold SEQ for existing pattern tools,
including length/rate settings. Patterns support up to 64 steps; the editor
shows sixteen at a time. Ties, chords and parameter locks retain their behavior.
Each pad shows its step number and note or HIT. Cream outline means the step
you are editing; a green underline means the playing step. HOLD is a tied step,
and a + after a note means a chord. In drum mode only the selected drum is shown:
turn DRUM to switch lanes. Disabled steps beyond the pattern length are dimmed.
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

Tap **GLO** to open SONG:

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
a different A–D source in the same section. PLAY on SONG starts the entire
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
