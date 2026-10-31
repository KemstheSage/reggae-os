/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 trk[3].step[0]=(step_t){{0},0,ST_NOTE,0,90,1,0};
 trk[0].step[0]=(step_t){{60,64},2,ST_NOTE,0,90};
 nv_set(&trk[0],0,1,40);
 assert(!project_save_as(0,"ORIGINAL"));
 project_store_t original=proj_slot[0];
 trk[3].step[0].hit=9;trk[0].step[0].note[0]=62;nv_set(&trk[0],0,1,110);
 assert(!project_save_as(1,"EDITED"));
 assert(!memcmp(&original,&proj_slot[0],sizeof original));
 /* Discard RAM caches and fetch from actual emulated NOR (power-cycle storage path). */
 memset(proj_slot,0,sizeof proj_slot);proj_fetch(0);proj_fetch(1);
 for(unsigned i=0;i<6;i++){
  unsigned slot=i%2;project_load(slot);advance(50);
  assert(trk[3].step[0].hit==(slot?9:1));
  assert(trk[0].step[0].note[0]==(slot?62:60));
  assert(nv_get(&motion,0,0,1,0)==(slot?110:40));
 }
 /* Sound slots intentionally leave the current sequence unchanged. */
 assert(up_store(0,"SOUND A")==0);trk[3].step[0].hit=17;
 assert(up_store(1,"SOUND B")==0);up_load(0);assert(trk[3].step[0].hit==17);
 go_home();tap(B_SAVE);assert(cur_page()->graph==GR_SLOTS);
 /* Full panel save / name / reload path in separate C/D slots. */
 web_enc(EN_K1,2);advance(200);assert(song.g[G_SLOT]==3);
 trk[3].step[0].hit=3;
 web_enc(EN_K4,1);advance(150);tap(B_OCTUP);
 assert(name_on()&&nm.slot==2);tap(B_OCTUP);assert(!name_on());
 trk[3].step[0].hit=12;
 web_enc(EN_K1,1);advance(200);assert(song.g[G_SLOT]==4);
 web_enc(EN_K4,1);advance(150);tap(B_OCTUP);
 assert(name_on()&&nm.slot==3);tap(B_OCTUP);assert(!name_on());
 web_enc(EN_K1,-1);advance(200);assert(ui.act==3);tap(B_OCTUP);
 assert(trk[3].step[0].hit==3);
 web_enc(EN_K1,1);advance(200);assert(ui.act==3);tap(B_OCTUP);
 assert(trk[3].step[0].hit==12);
 trk[3].step[0].hit=24;
 web_enc(EN_K4,1);advance(150);tap(B_OCTUP);
 assert(ui.confirm==CF_OVR_PROJ&&ui.confirm_trk==3);
 tap(B_OCTUP);assert(name_on()&&nm.slot==3);tap(B_OCTUP);
 web_enc(EN_K1,-1);advance(200);tap(B_OCTUP);assert(trk[3].step[0].hit==3);
 web_enc(EN_K1,1);advance(200);tap(B_OCTUP);assert(trk[3].step[0].hit==24);
 /* A rejected format keeps its dialog and cannot overwrite the saved slot. */
 project_store_t before_reject=proj_slot[3];
 name_open(NK_PROJ_SAVE,3);assert(motion.count);int8_t good=motion.event[0].value;
 motion.event[0].value=0;name_ok();assert(name_on());motion.event[0].value=good;name_close();
 assert(!memcmp(&before_reject,&proj_slot[3],sizeof before_reject));

 /* Tap through FX, release the button, play an effect, then explicitly leave. */
 go_home();for(unsigned i=0;i<10&&cur_page()->graph!=GR_PUNCH;i++)tap(B_FX);
 assert(cur_page()->graph==GR_PUNCH&&ui.lock==LAYER_FX);
 advance(2000);assert(ui.layer==LAYER_FX);
 web_keys(1u<<24);advance(150);assert(perf_act&PF_BIT(PF_GATE));
 web_keys(0);advance(300);assert(!perf_act&&ui.layer==LAYER_FX);
 tap(B_FX);assert(!ui.layer&&!ui.lock&&cur_page()->graph==GR_FX);
 web_buttons(1u<<B_FX);advance(650);web_buttons(0);advance(500);
 assert(ui.layer==LAYER_FX&&ui.lock==LAYER_FX&&cur_page()->graph==GR_PUNCH);
 tap(B_HOME);assert(!ui.layer&&!ui.lock);
 /* All standard browsing, even from an old stock patch, stays in the bank/user list. */
 song.sel=0;go_home();select_engine(REGGAE_BANK[2].engine);preset_step(1);assert(reggae_index(TSEL)>=0);
 for(unsigned i=0;i<NPAGES;i++)if(PAGES[i].graph==GR_BROWSE){ui.page=i;ui.home=0;break;}
 list_set(0);for(unsigned i=0;i<NELEM(REGGAE_BANK)+3;i++){preset_step(1);assert(reggae_index(TSEL)>=0||user_of(TSEL)<UP_SLOTS);}
 for(unsigned e=0;e<NENGINES;e++){reggae_select_engine(e);assert(reggae_index(TSEL)>=0||user_of(TSEL)<UP_SLOTS);}
 song.sel=3;go_home();select_engine(0);preset_step(1);assert(reggae_index(TSEL)>=0&&drum_track(TSEL));
 puts("PASS: independent project slots on NOR reload, sound-slot semantics, SAVE enters PROJECT, persistent punch page/exit, approved bank navigation");
}
