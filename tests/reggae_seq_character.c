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
 /* Every animated pose stays inside the dedicated header bay. */
 for(unsigned pose=0;pose<32;pose++) {
  ui_prefs=pose&1?PREF_ANIM_OFF:0;song.playing=1;
  TSEL->seq_idx=pose%16;TSEL->v[0].active=(pose/2)%2;
  cv_begin(240,56,RG_CASE);uint16_t blank=cv_px[0];
  reggae_seq_record(pose&1);
  for(unsigned y=0;y<56;y++)for(unsigned x=0;x<240;x++)
   if(x<16||x>48||y<19||y>50)assert(cv_px[y*240+x]==blank);
 }
 puts("PASS: native record character reacts to organ/drum voices and returns to idle; reduced-motion face stays responsive");
}
