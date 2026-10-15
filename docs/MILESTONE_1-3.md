# First playable REGGAE-OS prototype — 15 October 2026

Milestones 1–3 now have a working local organ/bass implementation; remaining
verification limits are explicit in TESTING.md. This is not the completed roadmap.

## Implemented

Four original patches: Roots Organ, Rocksteady, Deep Roots and Digital Bass.
Existing WHEEL and ANALOG DSP are reused. Four hardware encoders change sound and
native 240×240 artwork. Organ drawbars/rotor and bass cabinets respond to parameters
and the firmware peak meter, at a bounded 10 Hz. Preset recall/save uses existing NOR
storage. MIDI, effects and deeper editing remain available.

## Files

`firmware/src/reggae_engines.c` defines patches; `reggae_ui.c` renders artwork.
Integration changes: engines.c, main.c, ui.c, ui_draw.c and category.c. Emulator
changes are in web/emu/index.html; editor mock bank additions in web/editor.html.
Tests: tests/reggae_test.c, reggae_wasm.mjs, reggae_demo.c and updated ui_test.c.
Build/support scripts: tools/reggae/, tools/build.py and .github/workflows/reggae.yml.
README, BUILDING, HARDWARE, SOUND_DESIGN, UI_DESIGN, TESTING and ROADMAP describe the
implemented state. Upstream README and licence notices are preserved.

## Builds and tests

Original and modified pi32v2 hardware packages and WASM emulator built successfully.
Final app: 513,808 bytes; data/BSS 61,000/73,728; pool 328,896/368,640.
20 audio cases pass finite/non-silent output, bounded peaks, release and persistence
checks. All match native/WASM PCM exactly. Upstream emulator regression passes.
Full host suite remains non-green: baseline performance/font failures are documented;
project/motion regressions were corrected and focused suites rerun. See TESTING.md
and build/reggae/evidence for the detailed evidence rather than assuming full success.

## Try it

Open http://localhost:8790/ on this Mac; click Switch on. Turn PRESETS on HOME for
the four sounds, play the virtual keys and turn KNOB 1–4. If the server stops, run
`python3 -m http.server 8790 --bind 127.0.0.1 --directory build/emu`.

Original firmware-rendered musical WAV examples are in build/reggae/: roots-organ.wav,
rocksteady.wav, deep-roots-bass.wav and digital-bass.wav. Native screen images p0.png
through p3.png are 240×240; they are not browser mockups.

## Compatibility and next priority

Hardware: COMPILES, HARDWARE UNTESTED. Desktop: EMULATOR TESTED. Architecture:
CONFIRMED from source. Physical flashing/recovery, audio timing, MIDI electronics
and EP-40 sampling levels remain untested. Public HTTPS hosting is available at https://kemsthesage.github.io/reggae-os/.
Actual iPhone Safari verification remains pending. CI is prepared but has not run on GitHub.

Next: audition and refine organ articulation/bass levels; then implement melodica,
electric piano, brass/skank and siren using existing engines, followed by dedicated
dub-effect controls and graphics. Twelve curated patches and genre banks remain pending.
