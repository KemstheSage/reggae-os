static void reggae_seq_tools(void);
static void reggae_live_panel(int mixer);
static int reggae_seq_euclid(void);
static void layer_cards(uint32_t l);
/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS pattern / arrangement workspace. Existing sequencer, project sources,
 * input mappings and persistence; 240x124 scratch strip, no new audio state. */
static int reggae_seq_screen(void)
{
    return ui.layer==LAYER_SEQ || ui.layer==LAYER_GLO || ui.layer==LAYER_FX || (!ui.home && (cur_page()->graph == GR_SONG ||
        cur_page()->graph == GR_ROLL || cur_page()->graph == GR_EVENTS || cur_page()->graph == GR_TOOLS || cur_page()->graph == GR_CHORD || cur_page()->graph == GR_MIX4));
}
/* An original record creature. Voice activity opens its mouth; the outer groove
 * marker follows the sequence while its face stays readable. UI-rate work only. */
static int reggae_seq_singing(int arrange)
{
    for(unsigned t=0;t<NTRK;t++)if(arrange || t==song.sel)
        for(unsigned v=0;v<NVOICE;v++)if(trk[t].v[v].active)return 1;
    return 0;
}
static void reggae_seq_record(int small)
{
    int motion=!(ui_prefs&PREF_ANIM_OFF),sing=reggae_seq_singing(small);
    int ph=motion&&song.playing?(TSEL->seq_idx%16u)*64:128;
    const int cx=32;
    int cy=33-(motion&&sing?1:0),r=13;
    int ey=cy-3,blink=motion&&!sing&&fm1_ms%4200u<160u;
    int gaze=motion&&song.playing?(int)((TSEL->seq_idx/4u)%3u)-1:0;
    reggae_disc(cx,cy,r,RG_WOOD);reggae_disc(cx,cy,r-2,RG_BG);
    reggae_ring(cx,cy,r-3,RG_EDGE,RG_BG);reggae_disc(cx,cy,9,RG_GREEN);
    int x=cx+SINE[ph]*(r-2)/32768,y=cy-SINE[(ph+256)&1023]*(r-2)/32768;
    reggae_disc(x,y,1,RG_GOLD);
    for(int side=-1;side<=1;side+=2) {
        int ex=cx+side*4;
        if(blink)cv_rect(ex-2,ey,5,1,RG_INK);
        else {reggae_disc(ex,ey,3,RG_INK);reggae_disc(ex+gaze,ey,1,RG_BG);}
    }
    if(sing) {
        cv_rrect(cx-3,cy+3,7,5,2,RG_BG,RG_GREEN);
        cv_rect(cx-1,cy+6,3,2,RG_RED);
    } else {
        cv_line(cx-3,cy+4,cx-1,cy+6,RG_BG);
        cv_line(cx-1,cy+6,cx+2,cy+6,RG_BG);
        cv_line(cx+2,cy+6,cx+4,cy+4,RG_BG);
    }
    /* Keep the complete animated silhouette inside its own header bay. */
    cv_rect(16,sing&&motion?30:39,3,3,RG_INK);
    cv_rect(46,sing&&motion?30:39,3,3,RG_INK);
    cv_rect(23,49,6,2,RG_GOLD);cv_rect(36,49,6,2,RG_RED);
}
/* A tabletop and a shallow metal chassis, drawn in the existing three strips.
 * The editable grid keeps native glyphs, with narrower blank step gaps. */
#define RG_DESK RGB(103,66,40)
#define RG_CASE RGB(63,73,64)
#define RG_CASE_EDGE RGB(115,121,97)
static void reggae_seq_machine(int y0,int height)
{
    cv_begin(240,height,RG_DESK);
    for(int y=0;y<240;y+=12) {
        cv_rect(0,y-y0,240,1,RGB(82,51,32));
        cv_rect((y*7)%59,y+4-y0,54,1,RGB(123,80,46));
        cv_rect(151+(y%19),y+7-y0,62,1,RGB(91,56,33));
    }
    /* A short cable and rubber feet establish the physical tabletop silhouette. */
    cv_rect(180,3-y0,32,3,RG_BG);cv_rect(210,3-y0,3,10,RG_BG);
    cv_rect(207,9-y0,9,5,RG_WOOD);
    cv_rect(25,235-y0,18,5,RG_BG);cv_rect(192,235-y0,18,5,RG_BG);
    cv_rrect(13,17-y0,219,221,5,RGB(42,32,23),RG_DESK);
    cv_rrect(9,12-y0,218,225,5,RG_CASE_EDGE,RG_DESK);
    cv_rect(11,15-y0,214,219,RG_CASE);
    cv_rect(12,14-y0,212,1,RG_INK);
    cv_rect(12,234-y0,212,2,RG_EDGE);
    for(int x=12;x<=224;x+=212) {
        reggae_disc(x,232-y0,2,RG_CASE_EDGE);
        cv_line(x-1,232-y0,x+1,232-y0,RG_BG);
    }
}
static int reggae_seq_knob(unsigned k)
{
    if(cur_page()->graph==GR_SONG) {
        unsigned row=ui.song_row,lane=ui.song_trk%NTRK;
        if(k==0)return row*127/(CHAIN_ROWS-1);
        if(k==1)return lane*127/(NTRK-1);
        if(row>=chain_config.count)return 0;
        if(k==2){unsigned slot=chain_config.row[row].slot[lane];return slot<4?(slot+1)*25:0;}
        return clamp((chain_config.row[row].repeat-1)*127/15,0,127);
    }
    const step_t *st=&seq_steps(TSEL)[ui.cursor];
    if(k==0)return TSEL->p[P_SLEN]>1?ui.cursor*127/(TSEL->p[P_SLEN]-1):0;
    if(drum_track(TSEL)) {
        if(k==1)return ui.lane*127/(NLANE-1);
        return ((k==2?step_lanes(st):step_accents(st))&(1u<<ui.lane))?127:0;
    }
    if(k==1)return st->n?st->note[0]:0;
    if(k==2)return step_on(st)?clamp(note_len(TSEL,ui.cursor)*127/64,0,127):st->time==ST_TIE?64:0;
    return ((st->flags&SF_ACCENT)?42:0)+((st->flags&SF_SLIDE)?85:0);
}
static void reggae_seq_card_value(unsigned k,const char *label,const char *value,int ratio,uint16_t ink)
{
    int x=16+(int)k*52,hot=ui.hot_t&&ui.hot_col==k;
    /* Shadow, cap and pointer make the four existing hardware knobs tangible. */
    reggae_disc(x+27,11,9,RG_EDGE);reggae_disc(x+26,9,8,RG_BG);
    reggae_ring(x+26,9,7,hot?RG_INK:RG_CONTROL_COLOR[k],RG_BG);
    int ph=(640+ratio*6)&1023;
    cv_line(x+26,9,x+26+SINE[ph]*5/32768,9-SINE[(ph+256)&1023]*5/32768,RG_INK);
    cv_text_in(x,19+CAP_IN(XS,9),52,&AF_XS,label,RG_INK,RG_CASE);
    cv_rrect(x+2,29,48,14,2,RG_BG,RG_CASE);
    char fitted[24];text_fit(fitted,sizeof fitted,value,&AF_XS,44);
    cv_text_in(x+3,29+CAP_IN(XS,14),46,&AF_XS,fitted,ink,RG_BG);
}
static void reggae_seq_card(unsigned k,const char *label,const char *value)
{
    reggae_seq_card_value(k,label,value,reggae_seq_knob(k),RG_CONTROL_COLOR[k]);
}
static void reggae_seq_formatted_card(uint32_t k,const char *label,const char *value,
                                      const char *unit,uint16_t color,int32_t ratio)
{
    char b[32];str_cpy(b,value,sizeof b);
    unsigned n=str_len(b);
    if(unit && unit[0] && !str_eq(unit,label) && n+2<sizeof b) {
        str_cpy(b+n," ",sizeof b-n);n++;
        str_cpy(b+n,unit,sizeof b-n);
    }
    reggae_seq_card_value(k,label,b,ratio<0?0:clamp(ratio*127/1000,0,127),
                         color==T_DIM?RG_EDGE:RG_CONTROL_COLOR[k]);
}
static void reggae_arrangement(void)
{
    unsigned cursor=ui.song_row,base=(chain.running?chain.row:cursor)/4*4,lane=ui.song_trk%NTRK;
    char b[16];
    for(unsigned j=0;j<4 && base+j<=chain_config.count && base+j<CHAIN_ROWS;j++) {
        unsigned row=base+j;int y=5+(int)j*29;
        int used=row<chain_config.count,selected=row==cursor;
        if(selected)cv_rect(13,y,210,26,RG_PANEL);
        if(chain.running && row==chain.row)cv_rect(13,y,3,26,RG_GREEN);
        fmt_int(b,row+1);cv_text_in(16,y+6,23,&AF_S,b,selected?RG_GOLD:RG_INK,selected?RG_PANEL:RG_BG);
        if(!used) {
            cv_text(46,y+5,&AF_M,"+ ADD SECTION",RG_GOLD);continue;
        }
        for(unsigned k=0;k<NTRK;k++) {
            unsigned slot=chain_config.row[row].slot[k];int x=45+(int)k*33;
            uint16_t c=RG_CONTROL_COLOR[k];
            if(slot>=4)c=RG_EDGE;
            else if(!graph_project_used(slot))c=RG_RED;
            cv_rrect(x,y+2,27,22,-2,selected&&lane==k?RG_INK:c,RG_BG);
            cv_rect(x+2,y+4,23,18,selected&&lane==k?RG_INK:RG_BG);
            b[0]=slot<4?'A'+slot:'-';b[1]=0;
            cv_text_in(x+2,y+5,23,&AF_M,b,selected&&lane==k?RG_BG:c,selected&&lane==k?RG_INK:RG_BG);
            if(slot<4&&!graph_project_used(slot))cv_rect(x+21,y+3,3,3,RG_RED);
        }
        fmt_int(b,chain_config.row[row].repeat);cv_text_in(181,y+6,24,&AF_S,b,RG_INK,selected?RG_PANEL:RG_BG);
        cv_text(211,y+6,&AF_S,"x",RG_WOOD);
    }
}
/* Drop one blank column per step, not font/note pixels. Retain the original
 * 21 note rows / eight drum lanes. One 480-byte row scratch; no new framebuffer. */
static void reggae_seq_inset_grid(int drums,int detail)
{
    uint16_t row[240];
    for(int y=0;y<124;y++) {
        memcpy(row,cv_px+y*240,sizeof row);
        cv_rect(0,y,240,1,(y+56)%12==0?RGB(82,51,32):RG_DESK);
        cv_rect(13,y,219,1,RGB(42,32,23));cv_rect(9,y,218,1,RG_CASE);
        uint16_t *dst=cv_px+y*240;
        int prefix=detail?36:drums?36:35,src=drums&&!detail?4:0;
        memcpy(dst+12,row+src,(unsigned)prefix*2);
        for(int step=0;step<16;step++)
            memcpy(dst+12+prefix+step*11,row+(detail?36:drums?40:35)+step*12,22);
        if(!drums&&!detail)dst[223]=row[227];
    }
    cv_frame(11,0,214,124,RG_EDGE);
    /* The original left cursor edge occupied the inter-step gap we removed. */
    if(drums && !detail && ui.cursor/16u==ui.bank)
        cv_frame(47+(ui.cursor%16u)*11,11+ui.lane*14,12,12,RG_INK);
}
static void reggae_chord_panel(void)
{
    const track_t *t=TSEL;uint8_t notes[CHORD_MAX];int32_t root;uint16_t mask;
    if(chord_kit(t)) {cv_text_in(16,40,204,&AF_S,"CHORDS ARE FOR MELODIC TRACKS",RG_INK,RG_BG);return;}
    if(!t->p[P_CHRD]) {cv_text_in(16,36,204,&AF_S,"ONE KEY / ONE CHORD",RG_INK,RG_BG);
        cv_text_in(16,60,204,&AF_XS,"TURN K3 TO DIA3 OR DIA7",RG_GOLD,RG_BG);return;}
    unsigned n=chord_make(t,60+t->p[P_ROOT],notes,&root,&mask);
    if(chord_last[song.sel].n) {n=chord_last[song.sel].n;root=chord_last[song.sel].root;mask=chord_last[song.sel].mask;
        memcpy(notes,chord_last[song.sel].note,n);}
    char name[24];chord_name(name,root,mask);
    cv_text_in(16,7,204,&AF_S,name,RG_GOLD,RG_BG);
    for(unsigned i=0;i<n;i++) {
        int x=40+i*50,y=65-(notes[i]%12);int active=0;
        for(unsigned v=0;v<NVOICE;v++)active|=t->v[v].active;
        cv_rect(x-1,y+12,2,34,RG_GREEN);reggae_disc(x,y,12,RG_CONTROL_COLOR[i]);
        reggae_disc(x-4,y-2,2,RG_BG);reggae_disc(x+4,y-2,2,RG_BG);
        cv_rect(x-3,y+4,7,active?4:1,RG_BG);
        note_name(name,notes[i]);cv_text_in(x-20,103,40,&AF_XS,name,RG_INK,RG_BG);
    }
    if(trk_vmode(t)!=V_POLY)cv_text_in(16,24,204,&AF_XS,"MONO VOICE: ROOT ONLY",RG_RED,RG_BG);
}
static void reggae_seq_draw(void)
{
    static uint32_t last;
    int tools=ui.layer==LAYER_SEQ,mixer=ui.layer==LAYER_GLO||(!ui.layer&&!ui.home&&cur_page()->graph==GR_MIX4),punch=ui.layer==LAYER_FX;
    int live=mixer||punch,chords=!ui.layer&&cur_page()->graph==GR_CHORD;
    int velocity=!ui.layer&&nv_on(),detail=!ui.layer&&detail_on();
    int events=!ui.layer&&cur_page()->graph==GR_EVENTS,cleanup=!ui.layer&&cur_page()->graph==GR_TOOLS;
    int arrange=!ui.layer&&cur_page()->graph==GR_SONG;
    int advanced=velocity||tools||detail||events||cleanup||live||chords;
    __typeof__(ux) saved=ux;
    char b[32],v[4][16];const char *label[4];
    if(!ui.force && (uint32_t)(fm1_ms-last)<100u)return;
    last=fm1_ms;cursor_fix();ux.mono=ux.light=0;ux.gen++;
    ux.bg=ux.surf=RG_BG;ux.text=RG_INK;ux.accent=RG_RED;
    ux.mid=RG_GREEN;ux.dim=RG_EDGE;ux.raise=RG_WOOD;
    ux.lane=RG_PANEL;ux.grid=RGB(39,48,39);
    reggae_seq_machine(0,56);
    cv_rrect(53,17,169,36,3,RG_BG,RG_CASE);
    reggae_seq_record(arrange);
    cv_text(57,16,&AF_S,velocity?"NOTE VELOCITY":mixer?"FOUR TRACK MIX":punch?"DUB PUNCH-IN":chords?"CHORD GARDEN":tools?(reggae_seq_euclid()?"EUCLID":"PATTERN TOOLS"):detail?"STEP DETAIL":events?"AUTOMATION":cleanup?"CLEANUP":arrange?"DUB ARRANGE":drum_track(TSEL)?"RIDDIM GRID":"GROOVE ROLL",RG_INK);
    fmt_int(b,song.g[G_BPM]);cv_text(57,28,&AF_S,b,RG_GOLD);
    cv_text(83,28,&AF_S,"BPM",RG_WOOD);
    cv_text(117,28,&AF_S,song.rec?"REC":song.playing?"PLAY":"STOP",song.rec?RG_RED:RG_GREEN);
    for(unsigned beat=0;beat<4;beat++) {
        int active=song.playing && (TSEL->seq_idx/4u)%4u==beat;
        cv_rect(174+beat*12,34,10,3,active?RG_GOLD:RG_EDGE);
    }
    if(arrange) {
        for(unsigned k=0;k<NTRK;k++) { b[0]='T';b[1]='1'+k;b[2]=0;
            cv_text_in(55+k*30,40,27,&AF_S,b,RG_CONTROL_COLOR[k],RG_BG); }
        cv_text(182,40,&AF_S,"LOOPS",RG_WOOD);
    } else {
        b[0]='T';b[1]='1'+song.sel;b[2]=0;cv_text(57,40,&AF_S,b,RG_GOLD);
        int idx=reggae_index(TSEL);
        text_fit(b,sizeof b,idx>=0?ENGINES[TSEL->eng_req]->presets[TSEL->preset].name:ENGINES[TSEL->eng_req]->name,&AF_S,138);
        cv_text_in(78,40,140,&AF_S,b,RG_INK,RG_BG);
    }
    cv_blit(0,0);
    if(velocity) {
        const step_t *s=&seq_steps(TSEL)[ui.cursor];uint32_t count=drum_track(TSEL)?8:s->n;
        reggae_seq_machine(56,124);cv_rect(12,0,212,124,RG_BG);cv_oy=0;
        for(uint32_t i=0;i<count;i++) {
            uint32_t item=drum_track(TSEL)?i+4:i;int x=12+i*(drum_track(TSEL)?27:54),w=drum_track(TSEL)?20:40;
            uint32_t v=nv_get(chain_motion(TSEL),song.sel,ui.cursor,item,nv_fallback(s,item));
            int h=nv_present(s,item)?(int)v*72/127:0;
            cv_rect(x,90-h,w,h,item==nv_item()?RG_GOLD:RG_GREEN);
            if(item>=4) {str_cpy(b,drum_lane_name(TSEL,i),4);}else note_name(b,s->note[i]);
            cv_text_in(x,94,w,&AF_S,b,RG_INK,RG_BG);
            fmt_int(b,v);cv_text_in(x,108,w,&AF_S,nv_present(s,item)?b:"--",RG_INK,RG_BG);
        }
        if(!count)cv_text(60,50,&AF_S,"NO NOTES IN STEP",RG_INK);
    } else if(arrange||events||tools||cleanup||live||chords) {
        reggae_seq_machine(56,124);cv_rect(12,0,212,124,RG_BG);cv_oy=0;
        if(live)reggae_live_panel(mixer);
        else if(chords)reggae_chord_panel();
        else if(arrange)reggae_arrangement();
        else if(events)graph_events(1);
        else if(tools)reggae_seq_tools();
        else {
            static const char *const action[]={"CLEAR PATTERN","RESET SOUND","DELETE SECTION","CLEAR SONG"};
            for(unsigned k=0;k<4;k++) {
                int y=8+k*27,sel=act_col()==k+1;uint16_t bg=sel?RG_PANEL:RG_BG;
                cv_rrect(16,y,204,24,2,bg,RG_BG);
                cv_text_in(18,y+CAP_IN(S,24),200,&AF_S,k==1&&ui.tl_clean?"CLEAN SOUND":action[k],sel?RG_GOLD:RG_INK,bg);
            }
        }
    } else {
        cv_begin(240,124,RG_BG);cv_oy=0;
        if(detail)graph_detail(TSEL,RG_GOLD);
        else if(drum_track(TSEL))graph_grid(TSEL,RG_GOLD);
        else {pr_follow(TSEL);graph_roll(TSEL,RG_GOLD);}
        reggae_seq_inset_grid(drum_track(TSEL),detail);
    }
    cv_blit(0,56);
    reggae_seq_machine(180,60);
    if(arrange) {
        unsigned row=ui.song_row,lane=ui.song_trk%NTRK;int used=row<chain_config.count;
        label[0]="SECTION";label[1]="TRACK";label[2]="PATTERN";label[3]="LOOPS";
        fmt_int(v[0],row+1);v[1][0]='T';v[1][1]='1'+lane;v[1][2]=0;
        unsigned slot=used?chain_config.row[row].slot[lane]:CHAIN_SILENT;
        str_cpy(v[2],used?slot<4?"A":"MUTE":"ADD",16);if(used&&slot<4)v[2][0]='A'+slot;
        if(used)fmt_int(v[3],chain_config.row[row].repeat);else str_cpy(v[3],"--",16);
    } else {
        const step_t *st=&seq_steps(TSEL)[ui.cursor];
        label[0]="STEP";fmt_int(v[0],ui.cursor+1);
        if(drum_track(TSEL)) {
            label[1]="DRUM";label[2]="HIT";label[3]="ACCENT";
            str_cpy(v[1],drum_lane_name(TSEL,ui.lane),16);
            str_cpy(v[2],step_lanes(st)&(1u<<ui.lane)?"ON":"OFF",16);
            str_cpy(v[3],step_accents(st)&(1u<<ui.lane)?"ON":"OFF",16);
        } else {
            label[1]="NOTE";label[2]=step_on(st)?"LENGTH":"TIME";label[3]="FEEL";
            if(st->n)note_name(v[1],st->note[0]);else str_cpy(v[1],"--",16);
            if(step_on(st))fmt_int(v[2],note_len(TSEL,ui.cursor));
            else str_cpy(v[2],st->time==ST_TIE?"TIE":st->time==ST_REST?"REST":"NOTE",16);
            str_cpy(v[3],st->flags&SF_ACCENT?st->flags&SF_SLIDE?"A+SL":"ACC":st->flags&SF_SLIDE?"SLIDE":"NORM",16);
        }
    }
    if(advanced) {
        seq_card_draw=reggae_seq_formatted_card;
        if(tools||live)layer_cards(mixer?LAYER_GLO:ui.layer);else draw_columns();
        seq_card_draw=0;
    } else for(unsigned k=0;k<4;k++)reggae_seq_card(k,label[k],v[k]);
    const char *hint=drum_track(TSEL)?"SEQ: DETAIL  GLO: SONG":"HOLD EDIT+SEQ: CLEAR";
    if(velocity)hint="K2: NOTE  K3: VEL  K4: INHERIT";
    else if(arrange) {
        hint=chain.running?"PLAY: STOP / SONG RUNNING":"PLAY: SONG / SAVE: PROJECT";
        if(ui.song_row>=chain_config.count)hint="KNOB 3: ADD / SAVE: PATTERNS";
        else {unsigned sl=chain_config.row[ui.song_row].slot[ui.song_trk%NTRK];
            if(sl<4&&!graph_project_used(sl))hint="PATTERN NOT SAVED: USE SAVE";}
    }
    if(detail)hint="PRESETS: STEP  SEQ: AUTO";
    if(events) {str_cpy(b,"OCT+: ",sizeof b);str_cpy(b+6,ev_act_name(),sizeof b-6);hint=b;}
    if(tools)hint=reggae_seq_euclid()?"OCT+: APPLY  OCT-: CANCEL":"KEYS: TOOLS  OCT-: UNDO";
    if(cleanup)hint="TURN TO PICK  OCT+: CONFIRM";
    if(mixer)hint=ui.layer?"WHITE F-B: SOLO  BLACK: MUTE":"C5:LEVEL D5:PAN E5:REVERB";
    if(punch)hint="WHITE: FX  BLACK: MUTE";
    if(chords)hint="K3: CHORD  K4: VOICING";
    char foot[64];text_fit(foot,sizeof foot,ui.msg_t?ui.msg:hint,&AF_XS,202);
    cv_text_in(16,45+CAP_IN(XS,9),204,&AF_XS,foot,RG_INK,RG_CASE);
    cv_blit(0,180);
    if(ui.msg_t)ui.msg_t--;if(ui.hot_t)ui.hot_t--;
    saved.gen+=2;ux=saved;ui.force=0;
}
