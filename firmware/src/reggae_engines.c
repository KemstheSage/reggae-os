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
    {"REED MELODICA", {4, 0, 0, 2, 73, 12, 2, 80}, {32, 70, 112, 34}, 5, 1, FX(0, 0, 0, 8), PAT(0)},
    {"BRASS STABS", {0, 7, 46, 0, 64, 8, 7, 78}, {23, 67, 80, 26}, 25, 0, FX(0, 0, 0, 9), PAT(0)},
    {"DUB SIREN", {3, 0, 0, 0, 100, 0, 8, 0}, {6, 64, 127, 42}, 0, 1, FX(0, 0, 48, 10), PAT(0)},
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
    {"DUB DRAWBARS", {1, 0, 0, -2, 0, 8, 10, 1}, {0, 64, 127, 35}, 0, 0, FX(0, 0, 35, 16), PAT(0)},
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


static const preset_t REGGAE_SAMPLE_PRESETS[] = {
    {"PIANO", {0, 0, 0, 1, 127, 0, 0, 0}, {0, 127, 127, 70}, 0, 0},
    {"PIANO", {0, 0, 0, 1, 127, 0, 0, 0}, {0, 127, 127, 70}, 0, 0},
    {"FLUTE", {2, 0, 0, 1, 127, 0, 0, 0}, {12, 80, 120, 60}, 0, 0},
    {"SAX", {3, 0, 0, 1, 127, 0, 0, 0}, {12, 80, 120, 60}, 0, 0},
    {"PIANO", {0, 0, 0, 1, 127, 0, 0, 0}, {0, 127, 127, 70}, 0, 0},
    {"PIANO CHOP", {0, 0, 0, 0, 105, 0, 7, 0}, {0, 82, 64, 25}, 0, 0, FX(0, 0, 0, 8), PAT(0)},
};

static const engine_t ENG_SAMPLE = {
    .name = "SAMPLE",
    .page_title = {"SET", "TONE"},
    .edit = {
        {"SET", F_ENUM, 0, SMP_NALL - 1, 0, SMP_ALL_NAMES, 0},
        {"TUNE", F_SEMI, -24, 24, 0, 0, 0},
        {"BITS", F_INT, 0, 127, 0, 0, 0},
        {"LOOP", F_ENUM, 0, 1, 1, N_ONOFF_E, 0},
        {"CUT", F_INT, 0, 127, 127, 0, 0},
        {"-", F_INT, 0, 0, 0, 0, 0},
        {"DRV", F_PCT, 0, 127, 0, 0, 0},
        {"-", F_INT, 0, 0, 0, 0, 0},
    },
    .presets = REGGAE_SAMPLE_PRESETS,
    .npresets = NELEM(REGGAE_SAMPLE_PRESETS),
    .note_on = sample_note_on,
    .render = sample_render,
    .knob = {P_E0, P_E4, P_ATK, P_REL},
    .sampled = 1,
};

/* Shared bank metadata: panel navigation, graphics and tests use the same IDs.
 * Existing factory indices stay put; new patches are appended only. */
enum { RG_ORGAN, RG_BASS, RG_REED, RG_PIANO, RG_BRASS, RG_STRING, RG_SIREN, RG_DRUM };
typedef struct { uint8_t engine, preset, kind; const char *category; uint8_t knob[4]; } reggae_sound_t;
static const reggae_sound_t REGGAE_BANK[] = {
    {7,5,RG_ORGAN,"ORGAN",{P_E0,P_E3,P_E7,P_E6}},
    {7,6,RG_ORGAN,"ORGAN",{P_E0,P_E3,P_E7,P_E6}},
    {0,12,RG_BASS,"BASS",{P_E4,P_E5,P_ED_FLT,P_E6}},
    {0,13,RG_BASS,"BASS",{P_E4,P_E5,P_ED_FLT,P_E6}},
    {0,14,RG_REED,"MELODICA",{P_ATK,P_E4,P_LD_PIT,P_REL}},
    {12,8,RG_PIANO,"E-PIANO",{P_E2,P_E4,P_CHOR,P_E6}},
    {4,5,RG_PIANO,"PIANO",{P_E4,P_DEC,P_E6,P_REL}},
    {0,15,RG_BRASS,"BRASS",{P_E4,P_ATK,P_ED_FLT,P_REL}},
    {9,9,RG_STRING,"SKANK",{P_E2,P_E3,P_E4,P_REL}},
    {0,16,RG_SIREN,"SIREN",{P_ED_PIT,P_LRATE,P_DLY,P_E4}},
    {12,9,RG_BASS,"FM BASS",{P_E2,P_E4,P_E3,P_LEVEL}},
    {7,7,RG_ORGAN,"DUB ORGAN",{P_E0,P_E3,P_E7,P_DLY}},
    {10,4,RG_DRUM,"DRUM KIT",{P_E2,P_E3,P_E4,P_E7}},
};
static int reggae_index(const track_t *t)
{
    uint32_t i;
    if (t->user) return -1; /* User patches retain upstream controls; no guessing their instrument identity. */
    for (i=0;i<NELEM(REGGAE_BANK);i++)
        if(t->eng_req==REGGAE_BANK[i].engine && t->preset==REGGAE_BANK[i].preset) return (int)i;
    return -1;
}
static int reggae_patch(const track_t *t) { return reggae_index(t)>=0; }
/* Extra standard track parameters that preset_t cannot hold. Called on factory load,
 * never on user-preset recall: normal storage already preserves these parameters. */
static void reggae_loaded(track_t *t)
{
    if(t->eng_req==0 && t->preset==14) { t->p[P_LRATE]=88; t->p[P_LD_PIT]=1; t->p[P_LFADE]=68; }
    if(t->eng_req==0 && t->preset==16) { t->p[P_LRATE]=59; t->p[P_LD_PIT]=24; }
}
