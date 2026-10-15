# Sound design — initial bank

All four patches contain original parameter choices in `firmware/src/reggae_engines.c`.
They reuse Felucca-Engines' MIT WHEEL/ANALOG DSP without modifying the submodule.
The adapter retains the complete original preset prefix, then appends our sounds.
No commercial recordings or new sample material are included.

| Preset | Method / articulation | HOME knobs |
| --- | --- | --- |
| Roots Organ | WHEEL hollow 16′/4′ registration, reduced low bars, no percussion, light click/drive, slow rotary; full sustain, short release | Registration, upper-bar tone, rotary off/slow/fast, drive |
| Rocksteady | WHEEL brighter 8′ and upper harmonics, restrained upper bars, soft 2nd-harmonic percussion, sharper click, stationary rotor | Same |
| Deep Roots | Band-limited triangle, no detuning or second-oscillator mix, low resonant-filter setting, gentle filter envelope, high sustain | Cutoff, resonance, filter-envelope amount, drive |
| Digital Bass | Square oscillator, brighter filter, larger filter-envelope transient, lower sustain and short release | Same |

Digital Bass is an intentionally synthetic subtractive approximation for dancehall,
not an FM patch or a claim to reproduce a specific recording. A dedicated FM bass
and wider keyboard library remain planned. Both basses use the upstream legato
voice mode: overlapping notes preserve articulation; separate notes retrigger.
GLIDE remains available through the original editing pages.

The [Hammond organ guide](https://www.hammondorganco.com/wp-content/uploads/2021/11/SK-PRO-05a-ORGAN-VOICE-SECTION.pdf)
documents drawbar mixtures and percussion. WHEEL implements nine sine partials,
free-running phases, foldback, single-trigger percussion, contact noise and a
low-cost dual-rotor modulation model. It is a useful physical basis, not a full
cabinet/microphone simulation. Roots Organ deliberately leaves percussion off:
the musician supplies short, alternating offbeat chord articulations. Rocksteady
provides a different percussive attack. These are provisional musical designs,
not authenticated Jackie Mittoo registrations.

No new effects DSP has been added. Upstream already supplies chorus, delay,
room/spring/hall reverbs, saturation, insert effects and performance echo/reverb
throws. Those remain available; dedicated REGGAE-OS effect screens are planned.
Audition the generated WAVs and instrument before judging stylistic authenticity.
