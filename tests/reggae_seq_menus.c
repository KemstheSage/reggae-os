/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(200);}
static void shot(const char *p){ui.msg_t=0;ui.force=1;advance(150);picture(p);assert(!seq_card_draw);}
int main(void){
 web_nor_erase();web_boot();advance(1000);tap(9);
 TSEL->step[0].time=ST_NOTE;TSEL->step[0].n=1;TSEL->step[0].note[0]=48;
 shot("build/reggae/menus-roll.ppm");
 tap(9);assert(detail_on()&&reggae_seq_screen());
 int chance=step_chance(&TSEL->step[0]);web_enc(EN_K1,-1);advance(150);
 assert(step_chance(&TSEL->step[0])!=chance);shot("build/reggae/menus-detail.ppm");
 tap(9);assert(cur_page()->graph==GR_EVENTS&&reggae_seq_screen());
 ui.ev_row=0;int enabled=motion_enabled(TSEL);web_enc(EN_K4,enabled?-1:1);advance(150);
 assert(motion_enabled(TSEL)!=enabled);shot("build/reggae/menus-auto.ppm");
 web_enc(EN_K1,100);advance(200);shot("build/reggae/menus-add.ppm");
 unsigned count=motion_count(TSEL);tap(B_OCTUP);assert(motion_count(TSEL)>count);
 shot("build/reggae/menus-lock.ppm");
 tap(9);web_buttons(1u<<9);advance(650);assert(ui.layer==LAYER_SEQ);
 int len=TSEL->p[P_SLEN];web_enc(EN_K1,1);advance(150);assert(TSEL->p[P_SLEN]!=len);
 shot("build/reggae/menus-tools.ppm");
 tools_key(TL_EUCLID);advance(150);assert(eu.on);
 unsigned hits=eu.hits;web_enc(EN_K1,hits?-1:1);advance(150);assert(eu.hits!=hits);
 shot("build/reggae/menus-euclid.ppm");
 eu_close(TSEL,0);web_buttons(0);advance(200);assert(!ui.layer);
 ui.home=0;for(unsigned p=0;p<NPAGES;p++)if(PAGES[p].graph==GR_TOOLS)ui.page=p;
 ui.force=1;advance(150);web_enc(EN_K1,1);advance(150);assert(act_col()==1);
 shot("build/reggae/menus-cleanup.ppm");
 ui.home=1;track_select(3);advance(150);tap(9);
 web_buttons(1u<<9);advance(650);assert(ui.layer==LAYER_SEQ);
 shot("build/reggae/menus-drum-tools.ppm");web_buttons(0);advance(200);
 assert(!song.playing);puts("PASS: detail, automation, held tools, Euclid and cleanup rendering preserve edits");
}
