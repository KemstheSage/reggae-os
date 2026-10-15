/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS sound bank adapters. Existing records/descriptors below are from
 * Felucca-Engines, Copyright (C) 2026 Leo Kuroshita, Huegelton Instruments (MIT).
 * Keep the upstream prefix byte-for-byte: stored factory indices remain valid.
 * No DSP fork, additional voice buffers, or additional engine identities. */

static const preset_t REGGAE_ANALOG_PRESETS[] = {
    {"SAW LEAD", {0, 12, 64, 0, 90, 30, 10, 64}, {4, 70, 100, 50}, 20, 1, FX(0, 10, 45, 30), PAT(4)},
    {"SOFT PAD", {0, 20, 64, 4, 60, 10, 0, 32}, {80, 90, 110, 95}, 10, 0, FX(0, 60, 20, 70), PAT(5)},
    {"SQR BASS", {1, 0, 0, 0, 50, 70, 40, 64}, {0, 60, 40, 30}, 30, 1, FX(5, 0, 10, 10), PAT(2)},
    {"PWM STR", {4, 8, 40, 0, 75, 20, 0, 48}, {60, 80, 110, 85}, 8, 0, FX(0, 50, 20, 60), PAT(5)},
    {"ACID", {0, 0, 0, 0, 50, 100, 25, 64}, {0, 55, 20, 30}, 48, 1, FX(20, 0, 45, 15), PAT(1)},
    {"SINE KEY", {3, 6, 50, 0, 127, 0, 0, 0}, {2, 80, 30, 70}, 0, 0, FX(0, 30, 25, 40), PAT(6)},
    {"RAVE", {4, 30, 64, 0, 85, 20, 30, 64}, {20, 80, 110, 60}, 10, 1, FX(30, 40, 30, 30), PAT(13)},
    {"SUB BASS", {3, 0, 0, 0, 40, 0, 20, 0}, {0, 60, 100, 20}, 0, 1, FX(0, 0, 0, 10), PAT(8)},
    {"PLUCK", {0, 8, 50, 0, 30, 40, 0, 64}, {0, 88, 0, 60}, 55, 0, FX(0, 20, 50, 30), PAT(3)},
    {"BRASS", {0, 10, 64, 0, 45, 20, 10, 64}, {35, 70, 90, 45}, 40, 0, FX(0, 20, 20, 40), PAT(6)},
    {"WIND", {0, 0, 0, 90, 30, 90, 0, 0}, {60, 90, 60, 80}, 50, 0, FX(0, 30, 30, 70), PAT(5)},
    {"STRINGS", {0, 25, 64, 0, 70, 10, 0, 32}, {70, 90, 115, 90}, 5, 0, FX(0, 60, 20, 70), PAT(5)},
    {"DEEP ROOTS", {0, 0, 0, 0, 52, 0, 4, 96}, {14, 98, 18, 43}, 8, 1, FX(0, 0, 0, 0), PAT(0)},
    {"DIGITAL BASS", {1, 0, 0, 0, 48, 0, 4, 64}, {9, 90, 0, 34}, 12, 1, FX(0, 0, 0, 0), PAT(0)},

};

static const engine_t ENG_ANALOG = {
    .name = "ANALOG",
    .page_title = {"OSC", "FLT"},
    .edit = {
        {"WAVE", F_ENUM, 0, 6, 0, N_ANALOG_WAVE, 0},
        {"DTN", F_INT, 0, 127, 10, 0, "ct"},
        {"MIX", F_PCT, 0, 127, 64, 0, 0},
        {"NOIS", F_PCT, 0, 127, 0, 0, 0},
        {"CUT", F_CUTOFF, 0, 127, 90, 0, 0},
        {"RES", F_PCT, 0, 127, 30, 0, 0},
        {"DRV", F_PCT, 0, 127, 0, 0, 0},
        {"KTR", F_PCT, 0, 127, 64, 0, 0},
    },
    .presets = REGGAE_ANALOG_PRESETS,
    .npresets = NELEM(REGGAE_ANALOG_PRESETS),
    .note_on = analog_note_on,
    .render = analog_render,
    .desc = analog_desc,
    .knob = {P_E4, P_E5, P_ATK, P_REL},
    .keep = 0x03,                /* the filter */
};

static const preset_t REGGAE_WHEEL_PRESETS[] = {
    /* name, {REG, SUB, BODY, TOP, PERC, CLICK, DRIVE, ROTR}, {A D S R}, fenv, mono */
    {"FULL ORGAN", {15, 0, 0, 0, 0, 30, 20, 1}, {0, 64, 127, 45}, 0, 0, FX(0, 0, 10, 35), PAT(5)},
    {"JAZZ PERC", {4, 0, 0, 0, 2, 50, 8, 1}, {0, 64, 127, 40}, 0, 0, FX(0, 0, 0, 25), PAT(3)},
    {"GOSPEL", {6, 0, 0, 0, 1, 60, 40, 2}, {0, 64, 127, 45}, 0, 0, FX(0, 0, 0, 40), PAT(6)},
    {"SOFT FLUTE", {1, 0, -2, 0, 0, 10, 0, 1}, {0, 64, 127, 55}, 0, 0, FX(0, 20, 15, 55), PAT(5)},
    {"ROCK DRIVE", {7, 0, 0, 0, 0, 70, 100, 2}, {0, 64, 127, 40}, 0, 0, FX(35, 0, 10, 25), PAT(4)},
    {"ROOTS ORGAN", {3, 0, 0, 0, 0, 10, 3, 0}, {0, 64, 127, 20}, 0, 0, FX(0, 0, 0, 0), PAT(0)},
    {"ROCKSTEADY", {4, 0, 0, 0, 4, 20, 5, 0}, {0, 64, 127, 18}, 0, 0, FX(0, 0, 0, 0), PAT(0)},

};

static const engine_t ENG_WHEEL = {
    .name = "WHEEL",
    .page_title = {"BARS", "TONE"},
    .edit = {
        {"REG", F_ENUM, 0, 15, 4, N_DRW_REG, 0},
        {"SUB", F_INT, -8, 8, 0, 0, 0},
        {"BODY", F_INT, -8, 8, 0, 0, 0},
        {"TOP", F_INT, -8, 8, 0, 0, 0},
        {"PERC", F_ENUM, 0, 6, 0, N_DRW_PERC, 0},
        {"CLICK", F_PCT, 0, 127, 40, 0, 0},
        {"DRV", F_PCT, 0, 127, 0, 0, 0},
        {"ROTR", F_ENUM, 0, 2, 1, N_DRW_ROTR, 0},
    },
    .presets = REGGAE_WHEEL_PRESETS,
    .npresets = NELEM(REGGAE_WHEEL_PRESETS),
    .note_on = wheel_note_on,
    .render = wheel_render,
    .knob = {P_E0, P_E4, P_E6, P_E7},
    .block = wheel_block,
};

/* Existing factory patches keep their original HOME mapping and UI. User patches
 * from these engines get the performance controls without changing their data. */
static int reggae_patch(const track_t *t)
{
    return (t->eng_req == 7u && (t->user || t->preset >= 5u)) ||
           (t->eng_req == 0u && (t->user || t->preset >= 12u));
}
