/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static unsigned active(track_t *t,unsigned pitch){for(unsigned i=0;i<NVOICE;i++)if(t->v[i].note==pitch&&t->v[i].gate)return 1;return 0;}
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
int main(void){
 web_nor_erase();web_boot();advance(1000);track_t *t=&trk[0];
 t->p[P_VOICE]=V_POLY;t->p[P_QUANT]=0;t->p[P_SLEN]=16;
 t->step[0]=(step_t){{60,64,67},3,ST_NOTE,0,96};
 assert(nt_set(t,0,0,1)==0); /* quarter step */
 assert(nt_set(t,0,1,8)==0); /* two steps */
 assert(nv_set(t,0,1,70)==0);assert(motion_valid(&motion));
 seq_step(t,&t->step[0],0,4096,SEQ_ROLLED);assert(active(t,60)&&active(t,64));
 nt_tick(t,1100);assert(!active(t,60)&&active(t,64));
 t->step[1]=(step_t){{72},1,ST_NOTE,0,96};seq_step(t,&t->step[1],1,4096,SEQ_ROLLED);
 assert(active(t,64)&&active(t,72));nt_tick(t,7100);assert(!active(t,64));
 seq_release(t);
 /* Older timer never cuts a newer live note of the same pitch. */
 seq_step(t,&t->step[0],0,4096,SEQ_ROLLED);trk_note_on(t,64,110);nt_tick(t,9000);assert(active(t,64));seq_release(t);trk_all_off(t);
 /* Engine-switch queued notes retain their timer; panic cancels repeats. */
 t->xf_on=1;t->xf=0;nt_add(t,72,90,2048,1024,1,0);uint32_t pending=nt_generation[0][72];engine_block(t);
 assert(nt_generation[0][72]==pending);assert(active(t,72));nt_tick(t,2048);assert(!active(t,72));
 nt_add(t,72,90,512,1024,4,0);trk_all_off(t);pending=nt_generation[0][72];nt_tick(t,4096);assert(nt_generation[0][72]==pending&&!active(t,72));
 /* A four-hit hat does not retrigger its simultaneous kick. */
 t=&trk[3];t->step[0]=(step_t){{0},0,ST_NOTE,0,96,5,0};t->p[P_QUANT]=0;
 assert(nt_set(t,0,6,4u<<5)==0);assert(nv_set(t,0,6,65)==0);
 unsigned kick=DRUM_LANE_NOTE[0],hat=DRUM_LANE_NOTE[2];
 uint32_t k=nt_generation[3][kick],h=nt_generation[3][hat];seq_step(t,&t->step[0],0,4096,SEQ_ROLLED);
 for(unsigned i=0;i<31;i++)nt_tick(t,128);
 assert(nt_generation[3][kick]==k+1&&nt_generation[3][hat]==h+4);
 seq_stop();for(unsigned i=0;i<NT_SLOTS;i++)assert(!nt_events[3][i].active);
 /* Both attributes persist, move with removed notes, and survive velocity reset. */
 assert(project_save(1)==0);project_load(1);assert(nt_get(&motion,3,0,6)==128&&nv_get(&motion,3,0,6,0)==65);
 assert(nv_set(t,0,6,0)==0);assert(nt_get(&motion,3,0,6)==128);
 t=&trk[0];step_toggle(t,&t->step[0],(uint8_t[]){60},1);assert(nt_get(&motion,0,0,0)==8&&nv_get(&motion,0,0,0,0)==70);
 /* Actual panel: one editor selects a note, field, then its value. */
 song.sel=0;tap(B_HOME);tap(B_SEQ);assert(cur_page()->graph==GR_ROLL);
 ui.cursor=0;nv_selected[0]=0;note_field=0;unsigned before=nv_get(&motion,0,0,0,0);
 web_enc(EN_K4,-10);advance(100);assert(nv_get(&motion,0,0,0,0)==before-10);
 web_enc(EN_K3,1);advance(100);web_enc(EN_K4,1);advance(100);assert((nt_get(&motion,0,0,0)&31)==9);
 web_enc(EN_K3,1);advance(100);web_enc(EN_K4,4);advance(100);assert((nt_get(&motion,0,0,0)>>5)==4);
 ui.force=1;ui.msg_t=0;advance(100);picture("build/reggae/note-timing-grid.ppm");
 assert(project_save(2)==0);project_load(2);assert(nt_get(&motion,0,0,0)==137);
 grid_hit(&trk[3],0,2,0);assert(!nt_get(&motion,3,0,6));
 puts("PASS: independent lengths across steps, newer note ownership, 4 hats/1 kick, stop, timing+velocity save/recall, note removal, grid controls");
}
