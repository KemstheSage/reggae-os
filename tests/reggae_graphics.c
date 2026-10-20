/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static uint32_t art_hash(void) {
    uint32_t h=2166136261u;
    for(unsigned i=56*240;i<180*240;i++)h=(h^web_fb[i])*16777619u;
    return h;
}
int main(void) {
    web_nor_erase();web_boot();advance(1000);
    for(unsigned i=0;i<13;i++) {
        char path[80];snprintf(path,sizeof path,"build/reggae/screen-%u.ppm",i);
        if(i){web_enc(EN_PRESET,1);advance(200);}
        web_midi(0x90,60,100);advance(100);picture(path);
        web_midi(0x80,60,0);advance(1500);
    }
    /* Dub Drawbars has the real slow rotary enabled. */
    web_enc(EN_PRESET,-1);advance(200);web_midi(0x90,60,100);advance(500);
    uint32_t a=art_hash();advance(300);assert(a!=art_hash());
    ui_prefs|=PREF_ANIM_OFF;ui.force=1;advance(500);a=art_hash();
    advance(300);assert(a==art_hash());
    web_midi(0x80,60,0);
    for(unsigned i=0;i<4;i++){web_enc(EN_PRESET,1);advance(200);}
    assert(reggae_index(TSEL)==2);
    web_enc(EN_K1,12);advance(110);
    assert(reggae_focus_active());picture("build/reggae/focus-cutoff.ppm");
    advance(1300);assert(!reggae_focus_active());picture("build/reggae/play-cutoff.ppm");
    web_enc(EN_K2,12);advance(110);assert(reggae_focus_active());
    picture("build/reggae/focus-resonance.ppm");
    web_enc(EN_PRESET,1);advance(110);assert(!reggae_focus_active());
    puts("PASS: focused parameter follows panel turns, expires, and clears on preset change");
    puts("PASS: real rotary changes the illustration; ANIM OFF freezes it; thirteen native screens captured");
    web_enc(EN_PRESET,1);advance(1500);assert(reggae_index(TSEL)==4);
    /* Render deterministic rest/play frames and inspect the actual mouth. */
    cv_begin(240,124,RG_BG);reggae_art(TSEL,RG_REED,0,1);
    uint16_t closed=cv_px[74*240+174];cv_blit(0,56);
    picture("build/reggae/melodica-rest.ppm");
    cv_begin(240,124,RG_BG);reggae_art(TSEL,RG_REED,6000,1);
    assert(cv_px[74*240+174]!=closed);cv_blit(0,56);
    picture("build/reggae/melodica-playing.ppm");
    cv_begin(240,124,RG_BG);reggae_art(TSEL,RG_REED,6000,0);
    assert(cv_px[74*240+174]==closed);
    puts("PASS: melodica mouth opens with level and rests with animations disabled");
    /* Every FX amount must affect its own native illustration. */
    for(unsigned k=0;k<4;k++) {
        TSEL->p[P_DIST+k]=0;
        cv_begin(240,124,RG_BG);reggae_effect_art(0,1,6000);cv_blit(0,56);a=art_hash();
        TSEL->p[P_DIST+k]=100;
        cv_begin(240,124,RG_BG);reggae_effect_art(0,1,6000);cv_blit(0,56);assert(a!=art_hash());
    }
    cv_begin(240,124,RG_BG);reggae_effect_art(0,0,6000);cv_blit(0,56);a=art_hash();
    fm1_ms+=333;
    cv_begin(240,124,RG_BG);reggae_effect_art(0,0,6000);cv_blit(0,56);assert(a==art_hash());
    /* Capture real firmware FX pages, including model-specific reverb bodies. */
    ui_prefs&=~PREF_ANIM_OFF;ui.home=0;rg_focus.valid=0;
    for(unsigned n=0;n<NPAGES;n++)if(str_eq(PAGES[n].title,"FX") || str_eq(PAGES[n].title,"DLY") || str_eq(PAGES[n].title,"REVERB")) {
        char path[100];ui.page=n;ui.force=1;TSEL->peak=6000;reggae_draw();
        snprintf(path,sizeof path,"build/reggae/character-%s.ppm",PAGES[n].title);picture(path);
        if(str_eq(PAGES[n].title,"REVERB"))for(unsigned model=0;model<3;model++) {
            song.g[G_RTYPE]=model;TSEL->peak=6000;ui.force=1;reggae_draw();
            snprintf(path,sizeof path,"build/reggae/character-reverb-%u.ppm",model);picture(path);
        }
    }
    puts("PASS: all four FX controls change artwork; ANIM OFF freezes movement; real FX pages captured");
    cv_begin(32,32,RG_BG);reggae_disc(16,16,10,RG_INK);
    assert(cv_px[16*32+16]==swap16(RG_INK));assert(cv_px[0]==swap16(RG_BG));
    unsigned blended=0;
    for(unsigned y=6;y<=26;y++)for(unsigned x=6;x<=26;x++) {
        assert(cv_px[y*32+x]==cv_px[y*32+32-x]);
        if(cv_px[y*32+x]!=swap16(RG_BG) && cv_px[y*32+x]!=swap16(RG_INK))blended++;
    }
    assert(blended>0);
    reggae_disc(-2,-2,8,RG_INK);reggae_line(-5,-5,35,35,RG_GREEN);
    puts("PASS: antialiased circle has solid centre, blended symmetric edges and clipped primitives");
    return 0;
}
