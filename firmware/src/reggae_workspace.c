/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS workspaces: shared firmware UI, not a browser overlay.
 * Existing parameter, storage and input functions remain the source of truth.
 * Three <=124-row strips; no framebuffer allocation or audio callback work. */
static void rg_caption(int x,int y,int w,const char *s,uint16_t ink)
{ char b[64];text_fit(b,sizeof b,s,&AF_XS,w);cv_text_in(x,y,w,&AF_XS,b,ink,RG_BG); }
static void rg_heading(const char *title,const char *sub)
{
    reggae_seq_machine(0,56);reggae_seq_record(0);
    cv_rrect(54,18,167,34,3,RG_BG,RG_CASE);
    char b[48];text_fit(b,sizeof b,title,&AF_S,140);cv_text(59,19,&AF_S,b,RG_INK);
    rg_caption(58,36,131,sub,RG_GREEN);
    rg_caption(192,36,26,song.rec?"REC":song.playing?"RUN":"STOP",song.rec?RG_RED:RG_GOLD);
    cv_rect(207,22,11,6,RG_EDGE);cv_rect(218,24,2,2,RG_EDGE);
    cv_rect(208,23,clamp(batt_shown(),0,4)*2,4,RG_INK);
    cv_blit(0,0);
}
static void rg_panel(void)
{ reggae_seq_machine(56,124);cv_rect(3,0,234,124,RG_BG);cv_bg=RG_BG;
  if(ui_style==ST_LINE){cv_rect(4,0,232,1,RG_EDGE);cv_rect(4,123,232,1,RG_EDGE);} }
static void rg_footer(const char *a,const char *b)
{
    reggae_seq_machine(180,60);
    rg_caption(16,8,204,a,RG_INK);rg_caption(16,27,204,b,RG_INK);
    cv_blit(0,180);
}
/* A selected record sleeve and its neighbours, rather than the stock seven-row list. */
static void rg_sleeve(int row,const char *tag,const char *name,int selected,int used)
{
    int y=3+row*40;uint16_t col=selected?RG_GOLD:RG_WOOD;
    cv_rrect(15,y,208,36,3,selected?RG_PANEL:RG_BG,RG_BG);
    cv_rect(18,y+3,30,30,col);reggae_disc(33,y+18,12,RG_BG);
    reggae_disc(33,y+18,5,used?RG_RED:RG_EDGE);
    rg_caption(53,y+3,156,tag,selected?RG_GOLD:RG_GREEN);
    rg_caption(53,y+18,156,name,used?RG_INK:RG_WOOD);
    if(selected)cv_rect(218,y+5,3,26,RG_GOLD);
}
static void rg_library(unsigned kind)
{
    unsigned count=0,cur=0;
    if(kind==GR_BROWSE)cur=preset_pos(&count);
    else if(kind==GR_USER){count=UP_SLOTS;cur=ui.uslot;}
    else if(kind==GR_PATS){count=pat_count();cur=pat_pick();}
    else {count=4;cur=clamp(song.g[G_SLOT]-1,0,3);}
    if(!count){rg_caption(18,48,204,"NO SOUNDS IN THIS LIST",RG_INK);return;}
    unsigned first=cur?cur-1:0;if(count>3 && first>count-3)first=count-3;
    for(unsigned r=0;r<3 && first+r<count;r++){
        unsigned k=first+r;char tag[24],name[32];int used=1;
        if(kind==GR_BROWSE){unsigned p,e=preset_at(k,&p);
            if(e==NENGINES){up_slot_label(tag,p);up_name(p,name);}
            else {str_cpy(tag,ENGINES[e]->name,sizeof tag);str_cpy(name,ENGINES[e]->presets[p].name,sizeof name);}
            if(favorite_has(e,p))str_cpy(tag+str_len(tag)," *",sizeof tag-str_len(tag));
        }else if(kind==GR_USER){up_slot_label(tag,k);used=up_used(k);if(used)up_name(k,name);else str_cpy(name,"EMPTY SLOT",sizeof name);}
        else if(kind==GR_PATS)pat_label(k,tag,name);
        else {str_cpy(tag,"PROJECT A",sizeof tag);tag[8]='A'+k;used=graph_project_used(k);
            const char *n=graph_project_name(k);str_cpy(name,used?(n[0]?n:"SAVED PROJECT"):"EMPTY SLOT",sizeof name);}
        rg_sleeve(r,tag,name,k==cur,used);
    }
}
static void rg_engine_keys(void)
{
    unsigned n=NENG_SHOWN<LY_INIT?NENG_SHOWN:LY_INIT;
    for(unsigned k=0;k<n+2;k++){
        int x=15+(k%4)*53,y=3+(k/4)*24,sel=k<n && eng_vis(k)==TSEL->eng_req;
        uint16_t color=sel?RG_GOLD:RG_GREEN;
        cv_rect(x,y,48,20,RG_PANEL);reggae_disc(x+6,y+6,2,color);
        const char *name=k<n?eng_abbr(ENGINES[eng_vis(k)]->name):k==n?"INIT":"CLEAN";
        rg_caption(x+3,y+8,43,name,sel?RG_GOLD:RG_INK);
    }
    if(eqa_count(TSEL))eqa_row(eqa_count(TSEL));
    else {char b[16];sound_name(TSEL,b);rg_caption(16,108,204,b,RG_GOLD);}
}
static void rg_scale_keys(void)
{
    for(unsigned k=0;k<16 && k<=(unsigned)TP[P_SCALE].max;k++){
        int x=15+(k%4)*53,y=3+(k/4)*30,sel=k==(unsigned)TSEL->p[P_SCALE];
        cv_rect(x,y+2,48,25,RG_PANEL);
        for(int j=0;j<3;j++)cv_rect(x+9+j*11,y+6,5,sel?7:3,sel?RG_GOLD:RG_GREEN);
        rg_caption(x+1,y+15,46,TP[P_SCALE].names[k],sel?RG_GOLD:RG_INK);
    }
}
static void rg_record_keys(void)
{
    for(unsigned k=0;k<4;k++) {
        int x=16+(k%2)*105,y=3+(k/2)*60;
        uint16_t color=k?RG_CONTROL_COLOR[k]:RG_RED;
        cv_rrect(x,y,100,55,3,RG_PANEL,RG_BG);
        reggae_disc(x+12,y+12,6,color);
        char key[2]={W_NOTE[k],0};cv_text_in(x+6,y+7,12,&AF_XS,key,RG_BG,color);
        rg_caption(x+23,y+6,72,k?RL_LABEL[k-1]:"CLEAR",RG_INK);
        const char *v=k?rl_name(k-1,rp_get(RL_F[k-1])):chain_busy()?"STOP FIRST":rec_clearable()?"SEQUENCE":"EMPTY";
        rg_caption(x+3,y+30,94,v,color);
    }
}
static void rg_patchbay(void)
{
    for(unsigned k=0;k<NMSLOT;k++) {
        const int16_t *v=&TSEL->p[P_M1SRC+3*k];int y=4+k*29,sel=k==mod_ui_slot;
        uint16_t col=sel?RG_GOLD:RG_GREEN;
        reggae_ring(18,y+11,5,col,RG_BG);reggae_ring(221,y+11,5,col,RG_BG);
        reggae_line(23,y+11,36,y+21,col);reggae_line(36,y+21,201,y+21,col);reggae_line(201,y+21,216,y+11,col);
        rg_caption(27,y,70,N_MSRC[clamp(v[0],0,MS_N-1)],RG_INK);
        rg_caption(99,y,73,mod_dst_name(TSEL,v[1]),RG_INK);
        char b[16];const char *unit;param_format(&TP[P_M1AMT],v[2],b,&unit);
        rg_caption(172,y,41,b,col);
    }
}
static void rg_clock(void)
{
    int phase=(!(ui_prefs&PREF_ANIM_OFF) && song.playing)?(TSEL->seq_idx%16)*64:0;
    int dx=SINE[phase]*27/32768;
    cv_rrect(79,8,82,96,5,RG_WOOD,RG_BG);cv_rect(86,15,68,81,RG_PANEL);
    reggae_line(120,81,120+dx,27,RG_INK);reggae_disc(120+dx,27,8,RG_GOLD);
    reggae_eye(108,68,6,0,0);reggae_eye(132,68,6,0,0);
    cv_rect(112,85,16,3,RG_RED);
    rg_caption(20,110,200,song.playing?"CLOCK RUNNING":"PLAY: START CLOCK",RG_INK);
}
static void rg_chorus(void)
{
    int phase=!(ui_prefs&PREF_ANIM_OFF)?(fm1_ms*(song.g[G_CRATE]+1)/32)&1023:0;
    int sway=SINE[phase]*song.g[G_CDEPTH]/32768/8;
    for(int k=0;k<2;k++){
        int x=62+k*64,y=19+(k?sway:-sway);uint16_t col=k?RG_WOOD:RG_GREEN;
        cv_rrect(x,y,53,75,8,col,RG_BG);reggae_eye(x+26,y+22,12,0,0);
        reggae_mouth(x+16,y+48,20,reggae_seq_singing(0)?6:0,col);
    }
    reggae_line(104,102,137,102,RG_INK);rg_caption(14,110,212,"RATE: SWAY / DEPTH: DISTANCE",RG_INK);
}
/* Technical graphs retain their mathematical meaning, in the new workbench.
 * Curves/algorithms are data visualisations; their former page layout is gone. */
static void rg_editor_panel(void)
{
    const page_t *pg=cur_page();track_t *t=TSEL;unsigned g=pg->graph;
    if(ui.layer==LAYER_EDIT){rg_engine_keys();return;}
    if(ui.layer==LAYER_SCL){rg_scale_keys();return;}
    if(ui.layer==LAYER_REC){rg_record_keys();return;}
    if(ui.home && home_tracks()){reggae_live_panel(1);return;}
    if(!ui.home && str_eq(pg->title,"CLOCK")){rg_clock();return;}
    if(!ui.home && str_eq(pg->title,"CHORUS")){rg_chorus();return;}
    if(g==GR_BROWSE||g==GR_USER||g==GR_PATS||g==GR_SLOTS){rg_library(g);return;}
    cv_oy=11;graph_ht=100;
    if(ui.home){cv_oy=0;graph_scope(RG_GOLD);}
    else switch(g){
    case GR_ADSR:graph_adsr(t,RG_GOLD);break;
    case GR_LFO:graph_lfo(t,RG_GREEN);break;
    case GR_SCALE:graph_scale(t,RG_GOLD);break;
    case GR_MOD:cv_oy=0;rg_patchbay();break;
    case GR_SLCR:graph_slicer(t,RG_GOLD);break;
    case GR_INS:graph_insert(t,RG_GREEN);break;
    case GR_FMOP:case GR_FMOP2:if(fop_ok())graph_fm6(t,RG_GOLD,fop_op%6u);break;
    case GR_FMENV:if(fop_ok())graph_fm6env(t,RG_GOLD);break;
#if FELUCCA_SLICE
    case GR_SLICES:cv_oy=0;graph_slices();break;
#endif
    default:
        if(pg->scope==SC_ENGINE && ENGINES[t->eng_req]==&ENG_WHEEL)graph_wheel(t,RG_GOLD);
        else if(pg->scope==SC_ENGINE && ENGINES[t->eng_req]==&ENG_SAMPLE && sample_wave.ready)graph_sample(RG_GREEN);
        else if(pg->scope==SC_ENGINE && t->eng_req==ENGI_FM6)graph_fm6(t,RG_GOLD,6);
        else {cv_oy=0;graph_scope(RG_GREEN);}break;
    }
    cv_oy=0;
}
static void rg_workspace_page(void)
{
    char title[32],sub[48],hint[64];
    if(ui.layer)str_cpy(title,ui.layer==LAYER_EDIT?"ENGINE WORKSHOP":ui.layer==LAYER_SCL?"SCALE GARDEN":"RECORD DESK",sizeof title);
    else if(ui.home)str_cpy(title,"SOUND WORKSHOP",sizeof title);
    else page_title(title);
    sound_name(TSEL,sub);unsigned n=str_len(sub);
    str_cpy(sub+n," / T",sizeof sub-n);n=str_len(sub);sub[n]='1'+song.sel;sub[n+1]=0;
    rg_heading(title,sub);rg_panel();
    if(!ui.home && cur_page()->scope==SC_ENGINE && ENGINES[TSEL->eng_req]==&ENG_SAMPLE)sample_wave_tick(TSEL);
    rg_editor_panel();
    /* Preserve LARGE as an expanded live readout, without restoring stock layout. */
    if(!ui.layer && large_kind()!=LK_OFF && ui.hot_t) {
        int16_t *v;const param_desc_t *d=ui.home?home_param(ui.hot_col,&v):page_desc(cur_page(),ui.hot_col,&v);
        if(d && d->label && d->label[0]!='-') {
            char value[32];reggae_value(d,*v,value,sizeof value);
            cv_rect(20,18,200,88,RG_PANEL);
            rg_caption(24,28,192,d->label,RG_GOLD);
            cv_text_in(24,54,192,&AF_M,value,RG_INK,RG_PANEL);
        }
    }
    cv_blit(0,56);
    reggae_seq_machine(180,60);seq_card_draw=reggae_seq_formatted_card;
    if(ui.layer)layer_cards(ui.layer);else draw_columns();seq_card_draw=0;
    str_cpy(hint,"ALGORITHM: PAGE / HOME: PLAY",sizeof hint);
    if(ui.layer)str_cpy(hint,ui.layer==LAYER_EDIT?"WHITE: ENGINE / BLACK: ACTION":"KEYS: SELECT / OCT-: UNDO",sizeof hint);
    else if(act_cols()) {char a[16],b[16];foot_hint(a,b);str_cpy(hint,a,sizeof hint);str_cpy(hint+str_len(hint)," / ",sizeof hint-str_len(hint));str_cpy(hint+str_len(hint),b,sizeof hint-str_len(hint));}
    if(ui.ml.on)ml_line(hint);
    rg_caption(16,46,204,ui.msg_t?ui.msg:hint,RG_INK);cv_blit(0,180);
}
static int32_t rg_credits(int y,int draw)
{
    y=menu_text("REGGAE-OS / GPL-3.0-only",y,RG_GOLD,draw)+8;
    y=menu_text("Built on Felucca. Copyright 2026 Leo Kuroshita and Hugelton Instruments.",y,RG_INK,draw);
    y=menu_text("github.com/KemstheSage/reggae-os",y,RG_GREEN,draw)+8;
    for(unsigned i=0;i<NELEM(MENU_CREDITS);i++)
        y=menu_text(MENU_CREDITS[i].text,y,MENU_CREDITS[i].kind==MC_TITLE?RG_GOLD:RG_INK,draw)+3;
    return menu_text("NO WARRANTY. Full licences and corresponding source accompany the release.",y+8,RG_INK,draw);
}
static void rg_settings(void)
{
    char b[48];unsigned tab=menu_tab(),first=mtab_first(tab),count=mtab_rows(tab),sel=ui.menu_sel-first;
    rg_heading(ui.menu==2?"LABEL CREDITS":ui.menu==3?"SYSTEM METERS":"SYSTEM DESK",ui.menu==1?MTAB_NAME[tab]:"REGGAE-OS");
    rg_panel();
    if(ui.menu==2){cv_scroll=1;rg_credits(-(int)ui.menu_scroll,1);cv_scroll=0;}
    else if(ui.menu==3){
        info_sig();static const char *const usbname[]={"OFF","WAIT","BUS","ENUM","MIDI"};
        rg_sleeve(0,"FIRMWARE","REGGAE-OS UI 0.7.1",1,1);
        rg_sleeve(1,"USB",usbname[info_usb()],0,usb.up);
        fmt_int(b,info.cpu);str_cpy(b+str_len(b)," % AUDIO CPU",sizeof b-str_len(b));rg_sleeve(2,"PROCESSING",b,0,1);
    } else {
        unsigned start=(sel/4)*4;
        for(unsigned r=0;r<4 && start+r<count;r++){
            unsigned row=first+start+r;int x=16+(r%2)*105,y=4+(r/2)*58,active=row==ui.menu_sel;
            cv_rrect(x,y,100,53,3,active?RG_PANEL:RG_BG,RG_BG);
            cv_rect(x+3,y+5,3,41,active?RG_GOLD:RG_EDGE);
            rg_caption(x+8,y+6,88,MI_NAME[row],RG_INK);
            const char *v=menu_valued(row)?menu_vname(row,menu_get(row)):"OCT+: OPEN";
            if(row==MI_HOLD){fmt_int(b,HOLD_MS[settings_hold%4u]);str_cpy(b+str_len(b)," ms",sizeof b-str_len(b));v=b;}
            if(row==MI_TUNE){fmt_int(b,song.g[G_TUNE]);str_cpy(b+str_len(b)," ct",sizeof b-str_len(b));v=b;}
            if(row==MI_LCLEAR){fmt_int(b,ml_count());str_cpy(b+str_len(b)," CC / CLEAR",sizeof b-str_len(b));v=b;}
            rg_caption(x+8,y+27,88,v,active?RG_GOLD:RG_GREEN);
        }
    }
    cv_blit(0,56);
    if(ui.menu==1){
        reggae_seq_machine(180,60);
        for(unsigned k=0;k<MTAB_COUNT;k++)cv_rect(34+k*36,6,27,3,k==tab?RG_GOLD:RG_EDGE);
        rg_caption(16,17,204,"ALGORITHM: TAB / PRESETS: ITEM",RG_INK);
        rg_caption(16,33,204,"OCT-/+: CHANGE / HOME: CLOSE",RG_INK);cv_blit(0,180);
    }else rg_footer(ui.menu==2?"PRESETS: SCROLL":"LIVE SYSTEM STATUS","OCT-: BACK / HOME: CLOSE");
}
static void rg_name(void)
{
    char title[24];nm_title(title);rg_heading("LABEL PRESS",title);rg_panel();
    /* A paper record label, then two rows matching the sixteen white keys. */
    cv_rect(12,5,216,39,RG_INK);
    for(unsigned i=0;i<NM_LEN;i++){
        int x=17+17*i;char c[2]={i<nm.len?nm.s[i]:' ',0};
        cv_text_in(x,15,16,&AF_S,c,RG_BG,RG_INK);
        if(i==nm.cur)cv_rect(x,32,14,3,RG_RED);
    }
    if(nm.cur==NM_LEN)cv_rect(220,15,3,20,RG_RED);
    for(unsigned i=0;i<16;i++){
        int x=13+(i%8)*27,y=51+(i/8)*25;
        cv_rect(x,y,24,21,nm.key==i+1?RG_GOLD:RG_PANEL);
        rg_caption(x,y+5,24,nm_group(i),nm.key==i+1?RG_BG:RG_INK);
    }
    rg_caption(12,105,216,nm.key?"TAP AGAIN: NEXT LETTER":"WHITE KEYS: TYPE LETTERS",RG_GREEN);cv_blit(0,56);
    reggae_seq_machine(180,60);
    rg_caption(16,3,204,"F#:LEFT G#:SPACE A#:RIGHT C#:DEL",RG_INK);
    rg_caption(16,17,204,nm.num?"D#: ABC / K1:MOVE K2:CHAR":"D#: 123 / K1:MOVE K2:CHAR",RG_INK);
    char category[32]="K3: ";str_cpy(category+4,nm.kind<=NK_USER_RENAME?CAT_NAME[nm.cat%CAT_N]:"RANDOM NAME",sizeof category-4);
    rg_caption(16,31,204,category,RG_GOLD);
    rg_caption(16,45,204,transport_busy()?"STOP TO SAVE / OCT-: CANCEL":"OCT+: SAVE / OCT-: CANCEL",RG_INK);cv_blit(0,180);
}
static void rg_dialog(unsigned counting)
{
    char a[24],b[24];
    rg_heading(ui.uboot?"FIRMWARE UPDATE":counting?"COUNT-IN":"CHECK BEFORE CUT",song.rec?"RECORD ARMED":"REGGAE-OS");rg_panel();
    if(ui.confirm){confirm_text(a,b);
        reggae_disc(120,31,22,RG_RED);reggae_eye(112,27,6,0,0);reggae_eye(128,27,6,0,0);
        cv_rect(114,39,13,2,RG_BG);rg_caption(10,66,220,a,RG_INK);rg_caption(10,88,220,b,RG_GOLD);
    } else {
        unsigned beat=counting?(cin_total-cin_left)%4:0;
        fmt_int(a,ui.uboot?ui.uboot:4-beat);
        reggae_ring(120,53,39,RG_GOLD,RG_BG);cv_text_in(80,37,80,&AF_L,a,RG_INK,RG_BG);
        if(counting)for(unsigned k=0;k<4;k++)reggae_disc(75+k*30,107,5,k==beat?RG_RED:RG_EDGE);
    }
    cv_blit(0,56);
    rg_footer(ui.confirm?"OCT-: CANCEL / OCT+: CONFIRM":ui.uboot?"RELEASE OCT-/+ TO CANCEL":"RECORDING STARTS AFTER 1",ui.confirm?"CHANGES APPLY ONLY WHEN CONFIRMED":counting?"PLAY: STOP":"KEEP POWER CONNECTED");
}
#ifndef REGGAE_WORKSPACE_HOOK
#define REGGAE_WORKSPACE_HOOK() ((void)0)
#endif
static void reggae_workspace(uint32_t counting)
{
    static uint32_t last;
    if(!ui.force && fm1_ms-last<100u)return;
    if(ui.menu||name_on()||ui.confirm)counting=0;
    last=fm1_ms;cursor_fix();REGGAE_WORKSPACE_HOOK();
    __typeof__(ux) saved=ux;
    ux.mono=ux.light=0;ux.bg=ux.surf=RG_BG;ux.text=RG_INK;
    ux.theme=RG_GOLD;ux.accent=RG_RED;ux.mid=RG_GREEN;ux.dim=RG_WOOD;
    ux.raise=RG_PANEL;ux.ink=RG_BG;ux.gen++;
    if(ui.uboot||ui.confirm||counting)rg_dialog(counting);
    else if(ui.menu)rg_settings();
    else if(name_on())rg_name();
    else rg_workspace_page();
    if(ui.msg_t && !--ui.msg_t && ui.msg2[0]){str_cpy(ui.msg,ui.msg2,sizeof ui.msg);ui.msg2[0]=0;ui.msg_t=60;}
    if(ui.hot_t)ui.hot_t--;if(ui.bpm_t)ui.bpm_t--;if(ui.help_t)ui.help_t--;
    saved.gen+=2;ux=saved;ui.force=0;
}

static void reggae_calibration(const char *what,const char *name)
{
    rg_heading("CONTROL CHECK","HARDWARE CALIBRATION");rg_panel();
    reggae_ring(120,43,29,RG_GOLD,RG_BG);reggae_line(120,43,140,22,RG_INK);
    rg_caption(12,82,216,what,RG_INK);rg_caption(12,103,216,name,RG_GOLD);cv_blit(0,56);
    rg_footer("TEACH EACH BUTTON AND KNOB","ANY PIANO KEY: CANCEL");
}
