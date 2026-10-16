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
    puts("PASS: real rotary changes the illustration; ANIM OFF freezes it; thirteen native screens captured");
    return 0;
}
