/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
int main(void){
 web_nor_erase();web_boot();advance(1000);track_t *t=TSEL;
 t->p[P_VOICE]=V_POLY;t->step[0]=(step_t){{60,64,67},3,ST_NOTE,0,96};
 assert(nv_set(t,0,0,25)==0&&nv_set(t,0,1,70)==0&&nv_set(t,0,2,120)==0);
 seq_step(t,&t->step[0],0,22050,SEQ_ROLLED);
 unsigned seen=0;for(unsigned i=0;i<NVOICE;i++)if(t->v[i].gate){
  unsigned note=t->v[i].note;assert(t->v[i].vel==(note==60?25:note==64?70:120));seen++;
 }assert(seen==3);advance(100);double energy=0;for(unsigned i=0;i<128;i++)energy+=out_l[i]*out_l[i]+out_r[i]*out_r[i];assert(energy>1e-6);seq_release(t);advance(1500);
 assert(motion_valid(&motion));assert(nv_get(&motion,0,0,0,96)==25);
 motion_set_enabled(t,0);motion_step(t,0,&motion,1);assert(nv_get(&motion,0,0,1,96)==70);
 assert(motion_set_lock(t,0,P_REV,30)==0);motion_clear_automation(t);
 assert(motion.count==3&&nv_get(&motion,0,0,0,0)==25);
 step_undo_take(t,1234);nv_set(t,0,0,45);motion_undo_done(t);undo_swap();assert(nv_get(&motion,0,0,0,0)==25);
 t->p[P_SLEN]=1;assert(tl_double(t,1)==2);assert(nv_get(&motion,0,1,2,0)==120);tl_half(t,2);assert(!nv_get(&motion,0,1,2,0));
 assert(project_save(0)==0);assert(*(uint32_t *)proj_wire.raw==PROJ_MAGIC_NV);
 project_store_t badwire=proj_wire;project_t decoded;
 uint32_t oldmagic=PROJ_MAGIC;memcpy(badwire.raw,&oldmagic,4);
 uint32_t sum=proj_hash(badwire.raw,PROJ_STORE_SIZE-4);memcpy(badwire.raw+PROJ_STORE_SIZE-4,&sum,4);
 assert(!proj_unpack(&decoded,badwire.raw,PROJ_STORE_SIZE)); /* no reserved records disguised as FUNA */
 int8_t good=motion.event[0].value;motion.event[0].value=0;assert(!motion_valid(&motion));motion.event[0].value=good;

 nv_clear(t,0);project_load(0);assert(nv_get(&motion,0,0,2,0)==120);
 /* UI: select exactly one note and edit without changing its neighbours. */
 for(unsigned n=0;n<6&&!nv_on();n++)tap(B_SEQ);assert(nv_on());ui.cursor=0;
 web_enc(EN_K2,1);advance(150);assert(nv_item()==1);
 web_enc(EN_K3,-1);advance(150);assert(nv_get(&motion,0,0,1,0)<70);
 assert(nv_get(&motion,0,0,0,0)==25&&nv_get(&motion,0,0,2,0)==120);
 ui.msg_t=0;ui.force=1;advance(150);picture("build/reggae/note-velocity.ppm");
 web_enc(EN_K4,-1);advance(150);assert(nv_get(&motion,0,0,1,0)==0);
 /* Removing the low note carries the upper notes' overrides with them. */
 step_toggle(t,&t->step[0],(uint8_t[]){60},1);
 assert(t->step[0].note[0]==64&&nv_get(&motion,0,0,0,0)==0&&nv_get(&motion,0,0,1,0)==120);
 step_clear(&t->step[0]);assert(!motion.count);
 /* Full pool rejects an additional edit without corrupting saved events. */
 for(unsigned i=0;i<MOTION_MAX;i++)assert(nv_set(&trk[i/64],i%64,0,80)==0);
 assert(nv_set(t,0,1,99)==2&&motion.count==128&&motion_valid(&motion));
 motion_reset(&trk[0]);motion_reset(&trk[1]);assert(!motion.count);
 /* Drum lanes independent, accent overridden only by an explicit note edit. */
 t=&trk[3];song.sel=3;t->step[0]=(step_t){{0},0,ST_NOTE,0,96,9,9};
 assert(nv_set(t,0,4,30)==0&&nv_set(t,0,7,110)==0);
 assert(nv_get(&motion,3,0,4,127)==30&&nv_get(&motion,3,0,7,127)==110);
 ui.lane=0;ui.cursor=0;ui.msg_t=0;ui.force=1;advance(150);picture("build/reggae/drum-velocity.ppm");
 /* Chain source selects its saved velocity data, independent of current edits. */
 chain.source[0].motion=motion;chain.running=1;chain.lane[3]=0;nv_set(t,0,4,90);
 assert(nv_get(chain_motion(t),3,0,4,127)==30);chain.running=0;
 assert(project_save(1)==0);nv_clear(t,0);project_load(1);assert(nv_get(&motion,3,0,4,0)==90);
 puts("PASS: individual notes/drums, UI selection/edit/inherit, project format/recall, full pool, deletion/clear, independent arrangement source");
}
