/* SPDX-License-Identifier: GPL-3.0-only: generate documentation from native firmware. */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static FILE *meta;static char labels[4][32],values[4][40];
static void collect(uint32_t k,const char *label,const char *v,const char *unit,uint16_t color,int32_t ratio){
 str_cpy(labels[k],label,32);str_cpy(values[k],v,40);(void)unit;(void)color;(void)ratio;
}
static void json(const char *s){fputc('"',meta);for(;*s;s++){unsigned c=(unsigned char)*s;if(c=='"'||c=='\\')fputc('\\',meta);if(c<32||c>=127)fprintf(meta,"\\u%04x",c);else fputc(c,meta);}fputc('"',meta);}
static void entry(const char *key,const char *title,const char *group,int custom){
 char path[160];ui.msg_t=ui.hot_t=0;rg_focus.valid=0;ui.force=1;advance(150);
 snprintf(path,sizeof path,"build/reggae/guide/%s.ppm",key);picture(path);
 if(!custom){memset(labels,0,sizeof labels);memset(values,0,sizeof values);seq_card_draw=collect;
  if(ui.layer)layer_cards(ui.layer);else if(!ui.home && cur_page()->graph==GR_MIX4)layer_cards(LAYER_GLO);else draw_columns();
  seq_card_draw=0;
  if(ui.home && !ui.layer && reggae_index(TSEL)>=0)for(unsigned k=0;k<4;k++)str_cpy(labels[k],RG_CONTROL_NAME[reggae_index(TSEL)][k],32);
 }
 fprintf(meta,"{\"id\":");json(key);fprintf(meta,",\"title\":");json(title);fprintf(meta,",\"group\":");json(group);
 fprintf(meta,",\"engine\":");json(ENGINES[TSEL->eng_req]->name);fprintf(meta,",\"knobs\":[");
 for(unsigned k=0;k<4;k++){if(k)fputc(',',meta);json(labels[k]);}fprintf(meta,"]}\n");
}
static void custom(const char *a,const char *b,const char *c,const char *d){const char *v[]={a,b,c,d};for(unsigned k=0;k<4;k++)str_cpy(labels[k],v[k],32);}
int main(void){
 web_nor_erase();web_boot();advance(1000);meta=fopen("build/reggae/guide/pages.jsonl","w");assert(meta);
 for(unsigned k=0;k<NELEM(REGGAE_BANK);k++){
  set_engine_of(TSEL,REGGAE_BANK[k].engine);apply_preset_to(TSEL,REGGAE_BANK[k].preset);ui.home=1;
  char key[32];snprintf(key,sizeof key,"sound-%02u",k);entry(key,ENGINES[TSEL->eng_req]->presets[TSEL->preset].name,"Sounds",0);
 }
 for(unsigned p=0;p<NPAGES;p++){
  unsigned variants=PAGES[p].scope==SC_ENGINE;
  for(unsigned e=0;e<NENGINES;e++){
   if(str_eq(ENGINES[e]->name,"-"))continue;
   set_engine_of(TSEL,e);apply_preset_to(TSEL,0);ui.home=0;ui.page=p;
   if(!page_visible(p))continue;
   if(PAGES[p].graph==GR_CHORD){TSEL->p[P_CHRD]=CH_DOM9;TSEL->p[P_VOICE]=V_POLY;chord_build(TSEL,60,(uint8_t[4]){0});}
   if(PAGES[p].graph==GR_ROLL){TSEL->step[0].time=ST_NOTE;TSEL->step[0].n=1;TSEL->step[0].note[0]=60;}
   char key[32],title[64];snprintf(key,sizeof key,"page-%02u-%02u",p,e);
   snprintf(title,sizeof title,"%s%s%s",PAGES[p].title,variants?" / ":"",variants?ENGINES[e]->name:"");
   if(PAGES[p].graph==GR_SONG){custom("SECTION","TRACK","PATTERN","LOOPS");entry(key,title,"Pages",1);}
   else if(PAGES[p].graph==GR_ROLL && !detail_on()){custom("STEP","NOTE","LENGTH","FEEL");entry(key,title,"Pages",1);}
   else if(PAGES[p].graph==GR_PUNCH){punch_page_enter();advance(100);entry(key,title,"Pages",0);layer_let_go(0);ui.layer=0;}
   else entry(key,title,"Pages",0);
   if(!variants)break;
  }
 }
 /* Extra modes not represented by a distinct entry in PAGES. */
 set_engine_of(TSEL,REGGAE_BANK[12].engine);apply_preset_to(TSEL,REGGAE_BANK[12].preset);
 ui.page=page_first(FAM_SEQ);ui.home=0;custom("STEP","DRUM","HIT","ACCENT");entry("drum-grid","RIDDIM GRID","Sequencer",1);
 set_engine_of(TSEL,REGGAE_BANK[0].engine);apply_preset_to(TSEL,REGGAE_BANK[0].preset);
 ui.page=page_first(FAM_TRK);for(unsigned m=1;m<3;m++){reggae_mix_mode=m;entry(m==1?"mixer-pan":"mixer-reverb",m==1?"MIXER / PAN":"MIXER / REVERB","Mixer",0);}
 reggae_mix_mode=0;ui.home=1;
 const unsigned keys[]={B_EDIT,B_SCL,B_REC,B_SEQ,B_FX,B_GLO};
 const char *names[]={"ENGINE WORKSHOP","SCALE GARDEN","RECORD DESK","PATTERN TOOLS","DUB PUNCH-IN","HELD MIXER"};
 for(unsigned k=0;k<6;k++){
  web_buttons(1u<<keys[k]);advance(700);char id[24];snprintf(id,sizeof id,"held-%u",k);entry(id,names[k],"Held controls",0);
  if(keys[k]==B_SEQ){tools_key(TL_EUCLID);advance(150);entry("euclid","EUCLID","Sequencer",0);eu_close(TSEL,0);}
  web_buttons(0);advance(400);if(keys[k]==B_FX){layer_let_go(0);ui.layer=0;}
 }
 for(unsigned tab=0;tab<MTAB_COUNT;tab++){
  ui.menu=1;ui.menu_sel=mtab_first(tab);custom("VALUE","VALUE","VALUE","VALUE");char id[24];snprintf(id,sizeof id,"settings-%u",tab);entry(id,MTAB_NAME[tab],"Settings",1);
 }
 ui.menu=2;custom("--","--","--","--");entry("credits","LABEL CREDITS","Settings",1);
 ui.menu=3;entry("info","SYSTEM METERS","Settings",1);ui.menu=0;
 name_open(NK_USER_SAVE,0);str_cpy(nm.s,"ROOTS ORGAN",sizeof nm.s);nm.len=11;nm.cur=3;
 custom("CURSOR","CHARACTER","CATEGORY / RANDOM","--");entry("name","LABEL PRESS","Saving",1);name_close();
 ui.confirm=CF_CLEAR_MOTION;custom("--","--","--","--");entry("confirm","CONFIRMATION","Saving",1);ui.confirm=0;
 ui.uboot=3;entry("update","UPDATE COUNTDOWN","Settings",1);ui.uboot=0;
 cin_total=4;cin_left=3;ui.force=1;reggae_workspace(1);picture("build/reggae/guide/count-in.ppm");cin_left=0;
 fprintf(meta,"{\"id\":\"count-in\",\"title\":\"COUNT-IN\",\"group\":\"Saving\",\"engine\":\"WHEEL\",\"knobs\":[\"--\",\"--\",\"--\",\"--\"]}\n");
 reggae_calibration("TURN RIGHT","KNOB 1");picture("build/reggae/guide/calibration.ppm");
 fprintf(meta,"{\"id\":\"calibration\",\"title\":\"CALIBRATION\",\"group\":\"Settings\",\"engine\":\"WHEEL\",\"knobs\":[\"PROMPT\",\"PROMPT\",\"PROMPT\",\"PROMPT\"]}\n");
 fclose(meta);puts("PASS: guide screenshots and knob metadata captured from firmware");
}
