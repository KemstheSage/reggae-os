/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(60);web_buttons(0);advance(100);}
static void key(unsigned k){web_keys(1u<<k);advance(80);web_keys(0);advance(80);}
static unsigned notes(track_t *t){unsigned n=0;for(unsigned i=0;i<NSTEP;i++)n+=t->step[i].n+!!t->step[i].hit;return n;}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 assert(reggae_index(&trk[0])==0&&reggae_index(&trk[1])==2&&reggae_index(&trk[2])==7&&reggae_index(&trk[3])==12);
 tap(11);assert(rg_rec_wait==1&&!song.playing&&song.rec==1);advance(1000);assert(!song.playing&&!notes(TSEL));
 picture("build/reggae/live-ready.ppm");
 tap(11);assert(!rg_rec_wait&&!song.rec&&!song.playing); /* cancel arm */
 tap(11);key(7);assert(song.playing&&!rg_rec_wait&&song.rec==1);assert(trk[0].step[0].n);
 picture("build/reggae/live-record.ppm");
 tap(11);assert(song.playing&&!song.rec);unsigned n=notes(TSEL);key(12);assert(notes(TSEL)==n);
 web_enc(EN_ALGO,1);advance(150);assert(song.sel==1);tap(11);key(7);assert(song.rec==2&&notes(TSEL));
 /* Switching track ends recording, preserving the existing layer. */
 for(unsigned i=0;i<2;i++){web_enc(EN_ALGO,1);advance(150);}assert(song.sel==3&&!song.rec&&song.playing);
 web_enc(EN_PRESET,1);advance(150);assert(reggae_index(TSEL)==12);
 tap(11);key(0);assert(song.rec==8&&notes(TSEL));
 tap(10);assert(!song.playing&&!song.rec&&!rg_rec_wait);
 advance(2000);assert(project_save(0)==0);unsigned hits=notes(&trk[3]);
 memset(trk[3].step,0,sizeof trk[3].step);project_load(0);assert(notes(&trk[3])==hits);
 /* MIDI note also starts armed internal recording on its own track. */
 for(unsigned i=0;i<3;i++){web_enc(EN_ALGO,-1);advance(150);}tap(11);assert(rg_rec_wait==1);
 web_midi(0x90,67,100);advance(100);web_midi(0x80,67,0);advance(100);assert(song.playing&&!rg_rec_wait);
 tap(10);advance(2000);
 /* PLAY starts an armed loop without a note; STOP cancels it. */
 tap(11);tap(10);assert(song.playing&&song.rec==1);tap(10);assert(!song.rec);
 tap(11);web_enc(EN_ALGO,1);advance(150);assert(!rg_rec_wait&&!song.rec&&!song.playing);
 for(unsigned t=0;t<NTRK;t++)for(unsigned v=0;v<NVOICE;v++)assert(!trk[t].v[v].active);
 puts("PASS: live arm/cancel, first-note capture, MIDI start, overdub, track handoff, drum recording, stop and project recall");
}
