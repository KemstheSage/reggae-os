/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS: original 240x240 artwork drawn by the firmware strip renderer.
 * No allocation or rendering in the audio ISR. 10 Hz, integer geometry only.
 * Reuses the existing peak meter and WHEEL's actual rotor phase. */
#define RG_BG RGB(15, 26, 28)
#define RG_PANEL RGB(28, 48, 47)
#define RG_INK RGB(244, 227, 182)
#define RG_GOLD RGB(241, 185, 62)
#define RG_GREEN RGB(60, 189, 130)
#define RG_RED RGB(231, 91, 60)

static int reggae_screen(void)
{
    return ui.home && ui_home_view == HV_SCOPE &&
           reggae_patch(TSEL);
}

static void reggae_disc(int x, int y, int r, uint16_t col)
{
    int dy, dx;
    /* Small solid discs: integer circle, only inside the 124-row canvas. */
    for (dy = -r; dy <= r; dy++) {
        for (dx = 0; dx * dx + dy * dy <= r * r; dx++) ;
        cv_rect(x - dx + 1, y + dy, 2 * dx - 1, 1, col);
    }
}

static void reggae_draw(void)
{
    static uint32_t last, signature;
    static int32_t envelope;
    track_t *t = TSEL;
    uint32_t now = fm1_ms, k, organ = t->eng_req == 7u;
    uint8_t old_mono = ux.mono, old_light = ux.light;
    uint32_t sig = song.sel * 107u + t->preset * 31u + t->eng_req + t->user * 991u;
    int32_t pk;
    char name[32], val[12];
    const char *unit;
    const engine_t *e = ENGINES[t->eng_req];
    if (!ui.force && (uint32_t)(now - last) < 100u) return;
    last = now;
    /* GREY uses luminance ramps; our fixed-colour screen needs colour ramps.
     * Restore the selected theme before any ordinary page is drawn. */
    ux.mono = ux.light = 0;
    ux.gen++;
    pk = clamp(t->peak, 0, 32767);
    t->peak = 0;
    envelope = pk > envelope ? pk : envelope * 3 / 4;
    if (ui.force || sig != signature) {
        envelope = pk;
        signature = sig;
        cv_begin(240, 56, RG_BG);
        cv_rect(8, 8, 4, 14, RG_GREEN);
        cv_text(19, 7, &AF_M, "REGGAE-OS", RG_GOLD);
        cv_text(166, 9, &AF_S, organ ? "ORGAN" : "BASS", RG_GREEN);
        if (t->user) up_name(t->user - 1u, name);
        else str_cpy(name, e->presets[t->preset % e->npresets].name, sizeof name);
        cv_text(12, 30, &AF_M, name, RG_INK);
        cv_blit(0, 0);
    }
    cv_begin(240, 124, RG_BG);
    if (organ) {
        cv_rect(18, 8, 150, 87, RG_PANEL);
        cv_frame(18, 8, 150, 87, RG_GOLD);
        for (k = 0; k < 9u; k++) {
            int x = 28 + (int)k * 15, bar = drw_level(t->p, k);
            if (k == 8 && t->p[P_E4]) bar = 0;
            cv_rect(x + 3, 17, 3, 38, RG_BG);
            cv_rect(x, 18 + bar * 4, 10, 5, k < 2 ? RG_RED : RG_INK);
        }
        for (k = 0; k < 14u; k++) {
            cv_rect(26 + k * 9, 68, 8, 18, envelope > 400 && k % 4 == 0 ? RG_GREEN : RG_INK);
            if (k % 7 != 2 && k % 7 != 6) cv_rect(31 + k * 9, 68, 4, 10, RG_BG);
        }
        cv_rect(28, 96, 6, 13, RG_GOLD);
        cv_rect(153, 96, 6, 13, RG_GOLD);
        cv_rect(179, 25, 44, 73, RG_PANEL);
        cv_frame(179, 25, 44, 73, RG_GREEN);
        for (k = 0; k < 4; k++) cv_rect(185, 75 + 4*k, 32, 1, RG_GOLD);
        reggae_disc(201, 48, 15, RG_BG);
        /* A snapshot of the high rotor: animation accelerates with the DSP. */
        { uint32_t ph = (ui_prefs & PREF_ANIM_OFF) ? 0 : drw_t[song.sel].rph[0] >> 22;
          int dx = SINE[ph & 1023u] * 12 / 32768;
          int dy = SINE[(ph + 256u) & 1023u] * 12 / 32768;
          cv_line(201-dx, 48-dy, 201+dx, 48+dy, RG_GOLD);
          reggae_disc(201, 48, 3, RG_RED); }
        cv_rect(22, 115, clamp(t->p[P_E6], 0, 127), 3, RG_RED);
    } else {
        int pulse = (ui_prefs & PREF_ANIM_OFF) ? 0 : envelope * 5 / 32768;
        int drive = clamp(t->p[P_E6], 0, 127);
        cv_rect(41, 6, 158, 108, RG_GOLD);
        cv_rect(45, 10, 150, 100, RG_PANEL);
        cv_rect(53, 16, 134, 17, RG_BG);
        cv_text_in(53, 17, 134, &AF_S, "SOUND SYSTEM", RG_INK, RG_BG);
        for (k = 0; k < 2; k++) {
            int x = 82 + k * 76;
            reggae_disc(x, 68, 27, RG_BG);
            reggae_disc(x, 68, 22 + pulse, RG_GREEN);
            reggae_disc(x, 68, 17 + pulse, RG_PANEL);
            reggae_disc(x, 68, 8 + pulse, drive > 60 ? RG_RED : RG_GOLD);
        }
        cv_rect(55, 100, 130, 4, RG_BG);
        cv_rect(55, 100, t->p[P_E4] * 130 / 127, 4, RG_GREEN);
        cv_rect(49, 115, 14, 5, RG_RED); cv_rect(177, 115, 14, 5, RG_RED);
    }
    cv_blit(0, 56);
    cv_begin(240, 60, RG_BG);
    for (k = 0; k < 4; k++) {
        static const char *const org[] = {"BARS", "TONE", "ROTOR", "DRIVE"};
        static const char *const bas[] = {"CUT", "RES", "ENV", "DRIVE"};
        int16_t *vp;
        const param_desc_t *d = home_param(k, &vp);
        int x = (int)k * 60, hot = ui.hot_t && ui.hot_col == k;
        param_format(d, *vp, val, &unit);
        if (*unit) { uint32_t n = str_len(val); str_cpy(val + n, unit, sizeof val - n); }
        cv_rect(x+3, 1, 54, 43, RG_PANEL);
        cv_text_in(x+3, 3, 54, &AF_S, organ ? org[k] : bas[k], hot ? RG_GOLD : RG_GREEN, RG_PANEL);
        cv_text_in(x+3, 19, 54, &AF_S, val, RG_INK, RG_PANEL);
        cv_rect(x+6, 37, 48 * (*vp-d->min) / (d->max-d->min), 3, hot ? RG_RED : RG_GOLD);
    }
    cv_text_in(0, 46, 240, &AF_S, ui.msg_t ? ui.msg : "PRESETS: SOUND    EDIT: DETAIL", RG_INK, RG_BG);
    cv_blit(0, 180);
    if (ui.hot_t) ui.hot_t--;
    if (ui.msg_t) ui.msg_t--;
    ux.mono = old_mono; ux.light = old_light; ux.gen++;
    ui.force = 0;
}
