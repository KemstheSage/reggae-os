/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(60);web_buttons(0);advance(100);}
int main(void){
 web_nor_erase();web_boot();advance(1000);tap(9);
 ui_prefs|=PREF_ANIM_OFF;ui.force=1;advance(150);
 uint16_t idle[240*56];memcpy(idle,web_fb,sizeof idle);
 picture("build/reggae/character-idle.ppm");
 web_keys(1u<<7);advance(150);assert(reggae_seq_singing(0));
 unsigned changed=0;for(unsigned y=5;y<50;y++)for(unsigned x=0;x<52;x++)changed+=idle[y*240+x]!=web_fb[y*240+x];
 assert(changed>10);picture("build/reggae/character-singing.ppm");
 web_keys(0);advance(4000);assert(!reggae_seq_singing(0));
 for(unsigned y=5;y<50;y++)for(unsigned x=0;x<52;x++)assert(idle[y*240+x]==web_fb[y*240+x]);
 /* The same actual-voice response works with the drum kit. */
 ui.home=1;track_select(3);advance(150);web_keys(1);advance(30);assert(reggae_seq_singing(0));
 web_keys(0);advance(4000);assert(!reggae_seq_singing(0));
 puts("PASS: native record character reacts to organ/drum voices and returns to idle; reduced-motion face stays responsive");
}
