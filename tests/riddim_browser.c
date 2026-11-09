/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(void) {
 web_nor_erase();web_boot();advance(1000);
 unsigned seen=0,e,k,total;
 for(unsigned p=0;p<NELEM(RIDDIM_ORDER);p++) {
  unsigned id=RIDDIM_ORDER[p];assert(!(seen&(1u<<id)));seen|=1u<<id;
  assert(riddim_position(id)==p);
  e=preset_all_at(p,&k);assert(e==REGGAE_BANK[id].engine&&k==REGGAE_BANK[id].preset);
  preset_go(p);advance(20);assert(reggae_index(TSEL)==id);assert(preset_all_pos(&total)==p);
 }
 preset_go(0);ui.home=1;
 for(unsigned p=1;p<=NELEM(RIDDIM_ORDER);p++){
  preset_step(1);advance(20);assert(reggae_index(TSEL)==RIDDIM_ORDER[p%NELEM(RIDDIM_ORDER)]);
 }
 preset_step(-1);assert(reggae_index(TSEL)==27);
 song.sel=3;ui.home=1;
 for(unsigned n=0;n<10;n++) {
  preset_step(1);advance(20);assert(REGGAE_BANK[reggae_index(TSEL)].kind==RG_DRUM);
 }
 int old=reggae_index(TSEL);preset_step(1);assert(old!=reggae_index(TSEL));preset_step(-1);assert(old==reggae_index(TSEL));
 puts("PASS grouped browser permutation, stored identities, forward/reverse wrapping, drum-track kit selection");
}
