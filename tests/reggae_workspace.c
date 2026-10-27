/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
static unsigned workspace_draws;
#define REGGAE_WORKSPACE_HOOK() (workspace_draws++)
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
static void shot(const char *p){ui.msg_t=0;ui.force=1;advance(150);picture(p);assert(!seq_card_draw);}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 web_buttons(1u<<B_HOME);advance(700);web_buttons(0);advance(200);assert(ui.menu==1);
 ui.menu_sel=MI_ANIM;ui.force=1;unsigned before=menu_get(MI_ANIM);
 web_enc(EN_K1,before?-1:1);advance(150);assert(menu_get(MI_ANIM)!=before);
 web_enc(EN_K1,before?1:-1);advance(150);assert(menu_get(MI_ANIM)==before);
 ui.menu_sel=MI_COLOR;ui.force=1;shot("build/reggae/workspace-settings.ppm");
 unsigned tab=menu_tab();web_enc(EN_ALGO,1);advance(200);assert(menu_tab()!=tab);
 web_enc(EN_PRESET,1);advance(200);assert(ui.menu_sel!=mtab_first(menu_tab()));
 tap(B_HOME);assert(!ui.menu);
 tap(B_SEQ);unsigned frames=workspace_draws;
 web_buttons(1u<<B_SCL);advance(650);assert(ui.layer==LAYER_SCL && workspace_draws>frames);
 shot("build/reggae/workspace-held-scale.ppm");web_buttons(0);advance(400);
 tap(B_ENV);assert(!ui.home && cur_page()->graph==GR_ADSR);
 int16_t *v;const param_desc_t *d=page_desc(cur_page(),0,&v);assert(d);
 int old=*v;web_enc(EN_K1,old<d->max?1:-1);advance(200);assert(*v!=old);
 shot("build/reggae/workspace-envelope.ppm");
 name_open(NK_USER_SAVE,0);advance(200);web_keys(1u);advance(70);web_keys(0);advance(900);
 assert(name_on() && nm.len);char saved[13];str_cpy(saved,nm.s,sizeof saved);
 shot("build/reggae/workspace-name.ppm");tap(B_OCTUP);assert(!name_on() && up_used(0));
 char actual[13];up_name(0,actual);assert(!strcmp(saved,actual));
 ui.confirm=CF_CLEAR_MOTION;ui.force=1;advance(150);tap(B_OCTDN);assert(!ui.confirm);
 for(unsigned p=0;p<NPAGES;p++)if(str_eq(PAGES[p].title,"CLOCK"))ui.page=p;
 ui.home=0;ui.force=1;shot("build/reggae/workspace-clock.ppm");
 for(unsigned p=0;p<NPAGES;p++)if(str_eq(PAGES[p].title,"CHORUS"))ui.page=p;
 ui.force=1;shot("build/reggae/workspace-chorus.ppm");
 unsigned char prior[sizeof web_fb];memcpy(prior,web_fb,sizeof web_fb);
 web_enc(EN_K2,20);advance(300);assert(memcmp(prior,web_fb,sizeof web_fb));
 puts("PASS: Settings hold/tab/selection/edit/close, envelope editing, physical-key naming/save, confirmation cancel, chorus redraw");
}
