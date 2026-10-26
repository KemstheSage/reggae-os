/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
static void shot(const char *p){ui.msg_t=0;ui.force=1;advance(150);picture(p);assert(!seq_card_draw);}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 tap(B_HOME);assert(!ui.home&&!ui.layer&&cur_page()->graph==GR_MIX4&&reggae_seq_screen());
 advance(2000);assert(!ui.layer&&cur_page()->graph==GR_MIX4);

 for(unsigned k=0;k<4;k++){int level=trk[k].p[P_LEVEL];web_enc(EN_K1+k,-10);advance(150);assert(trk[k].p[P_LEVEL]<level);}
 web_keys(1u<<1);advance(150);assert(trk[0].p[P_MUTE]);web_keys(0);advance(150);
 web_keys(1u<<2);advance(150);assert(perf_solo==2);shot("build/reggae/live-mixer.ppm");
 web_keys(0);advance(300);assert(!perf_solo&&!ui.layer);
 web_keys(1u<<2);advance(150);assert(perf_solo==2);
 tap(B_HOME);assert(!perf_solo);web_keys(0);advance(150);
 assert(cur_page()->graph==GR_TRK);int pan=TSEL->p[P_PAN];
 web_enc(EN_K2,1);advance(200);assert(TSEL->p[P_PAN]!=pan);
 tap(B_HOME);assert(cur_page()->fam==FAM_TRK);tap(B_HOME);assert(ui.home);
 web_keys(1u<<7);advance(150);assert(!kb_layer);web_keys(0);advance(150);

 web_buttons(1u<<B_FX);advance(650);assert(ui.layer==LAYER_FX&&reggae_seq_screen());
 web_keys((1u<<24)|(1u<<26));advance(150);assert((perf_act&PF_BIT(PF_GATE))&&(perf_act&PF_BIT(PF_MONO)));
 shot("build/reggae/live-punch.ppm");web_keys(0);web_buttons(0);advance(300);assert(!perf_held&&!perf_act);
 tap(B_SCL);tap(B_SCL);assert(cur_page()->graph==GR_CHORD&&reggae_seq_screen());
 web_enc(EN_K3,1);advance(150);assert(TSEL->p[P_CHRD]==1);
 int root=TSEL->p[P_ROOT];web_enc(EN_K1,1);advance(150);assert(TSEL->p[P_ROOT]!=root);
 web_enc(EN_K4,1);advance(150);assert(TSEL->p[P_VOIC]==1);
 TSEL->p[P_MUTE]=0;web_keys(1u<<7);advance(200);assert(chord_last[0].n==3);
 double energy=0;for(unsigned b=0;b<10;b++){web_render(128);for(unsigned i=0;i<128;i++)energy+=out_l[i]*out_l[i]+out_r[i]*out_r[i];}
 assert(energy>0.01);shot("build/reggae/live-chords.ppm");web_keys(0);advance(10000);
 for(unsigned v=0;v<NVOICE;v++)assert(!TSEL->v[v].active);
 puts("PASS: four level knobs, mute, solo, gate/mono keys, chord root/voicing, audible triad and release");
}
