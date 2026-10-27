/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static unsigned shots;
static void shot(const char *label){
 char path[160];snprintf(path,sizeof path,"build/reggae/skin/%03u-%s.ppm",shots++,label);
 ui.msg_t=0;ui.force=1;advance(150);picture(path);assert(!seq_card_draw);
}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 assert(palette_cur()==NPALETTES-1u);
 for(unsigned engine=0;engine<NENGINES;engine++){
  set_engine_of(TSEL,engine);advance(100);ui.home=0;
  for(unsigned page=0;page<NPAGES;page++){
   ui.page=page;if(!page_visible(page))continue;
   page_entered();shot(PAGES[page].title);
  }
 }
 ui.home=1;ui.layer=0;
 for(unsigned menu=1;menu<=3;menu++){ui.menu=menu;shot("settings");}
 ui.menu=0;
 const unsigned held[]={B_EDIT,B_SCL,B_REC};
 for(unsigned i=0;i<NELEM(held);i++){
  web_buttons(1u<<held[i]);advance(650);assert(ui.layer);
  shot("held-controls");web_buttons(0);advance(400);
 }
 ui.layer=0;ui.confirm=CF_CLEAR_MOTION;shot("confirm");ui.confirm=0;
 name_open(NK_USER_RENAME,0);shot("name");name_close();
 ui.uboot=3;shot("update-cancel");ui.uboot=0;
 printf("PASS: %u native firmware screens rendered under ASAN\n",shots);
}
