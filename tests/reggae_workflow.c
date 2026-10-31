/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 tap(B_FX);assert(cur_page()->graph==GR_PUNCH&&ui.lock==LAYER_FX);
 tap(B_FX);assert(cur_page()->graph==GR_FX&&!ui.lock);assert(reggae_fxscreen());
 ui.force=1;advance(150);picture("build/reggae/workflow-sends.ppm");
 int before=TSEL->p[P_DLY];web_enc(EN_K3,20);advance(100);assert(TSEL->p[P_DLY]==before+20);
 for(unsigned i=0;i<NPAGES;i++)assert(!(PAGES[i].fam==FAM_FX&&PAGES[i].graph==GR_SLCR));
 unsigned count=0;for(unsigned i=0;i<NPAGES;i++)count+=PAGES[i].fam==FAM_FX&&page_visible(i);
 for(unsigned i=1;i<count;i++){tap(B_FX);assert(cur_page()->graph!=GR_SLCR);}
 assert(cur_page()->graph==GR_PUNCH&&ui.lock==LAYER_FX);
 tap(B_HOME);tap(B_FX);assert(cur_page()->graph==GR_PUNCH);
 tap(B_HOME);song.sel=3;go_home();select_engine(0);preset_step(1);assert(drum_track(TSEL));
 tap(B_SEQ);assert(grid_on());ui.cursor=0;ui.lane=0;
 grid_hit(TSEL,0,0,1);grid_hit(TSEL,0,2,1);
 uint32_t hat=nv_get(&motion,3,0,6,nv_fallback(&TSEL->step[0],6));
 web_enc(EN_K4,-20);advance(100);
 assert(nv_get(&motion,3,0,4,0)>0&&nv_get(&motion,3,0,4,0)<hat);
 assert(nv_get(&motion,3,0,6,nv_fallback(&TSEL->step[0],6))==hat);
 unsigned vel=nv_get(&motion,3,0,4,0);assert(project_save(0)==0);
 nv_set(TSEL,0,4,127);project_load(0);advance(100);assert(nv_get(&motion,3,0,4,0)==vel);
 ui.msg_t=0;ui.force=1;advance(150);picture("build/reggae/workflow-grid.ppm");
 puts("PASS: FX enters punch, sends next and edit selected track; chopper outside FX; grid K4 edits only selected hit, project recall preserves velocity");
}
