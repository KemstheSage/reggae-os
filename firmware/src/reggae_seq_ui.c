/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS pattern / arrangement workspace. Existing sequencer, project sources,
 * input mappings and persistence; 240x124 scratch strip, no new audio state. */
static int reggae_seq_screen(void)
{
    return !ui.home && (cur_page()->graph == GR_SONG ||
        (cur_page()->graph == GR_ROLL && !detail_on()));
}
static void reggae_seq_card(unsigned k,const char *label,const char *value)
{
    int x=(int)k*60;
    cv_rect(x+5,3,50,2,RG_CONTROL_COLOR[k]);
    cv_text_in(x+2,10,56,&AF_S,label,RG_CONTROL_COLOR[k],RG_BG);
    cv_text_in(x+2,25,56,&AF_M,value,RG_INK,RG_BG);
}
static void reggae_arrangement(void)
{
    unsigned cursor=ui.song_row,base=(chain.running?chain.row:cursor)/4*4,lane=ui.song_trk%NTRK;
    char b[16];
    for(unsigned j=0;j<4 && base+j<=chain_config.count && base+j<CHAIN_ROWS;j++) {
        unsigned row=base+j;int y=5+(int)j*29;
        int used=row<chain_config.count,selected=row==cursor;
        if(selected)cv_rect(3,y,234,26,RG_PANEL);
        if(chain.running && row==chain.row)cv_rect(3,y,3,26,RG_GREEN);
        fmt_int(b,row+1);cv_text_in(8,y+6,23,&AF_S,b,selected?RG_GOLD:RG_INK,selected?RG_PANEL:RG_BG);
        if(!used) {
            cv_text(40,y+5,&AF_M,"+ ADD SECTION",RG_GOLD);continue;
        }
        for(unsigned k=0;k<NTRK;k++) {
            unsigned slot=chain_config.row[row].slot[k];int x=37+(int)k*37;
            uint16_t c=RG_CONTROL_COLOR[k];
            if(slot>=4)c=RG_EDGE;
            else if(!graph_project_used(slot))c=RG_RED;
            cv_rrect(x,y+2,31,22,-2,selected&&lane==k?RG_INK:c,RG_BG);
            cv_rect(x+2,y+4,27,18,selected&&lane==k?RG_INK:RG_BG);
            b[0]=slot<4?'A'+slot:'-';b[1]=0;
            cv_text_in(x+2,y+5,27,&AF_M,b,selected&&lane==k?RG_BG:c,selected&&lane==k?RG_INK:RG_BG);
            if(slot<4&&!graph_project_used(slot))cv_rect(x+24,y+3,3,3,RG_RED);
        }
        fmt_int(b,chain_config.row[row].repeat);cv_text_in(190,y+6,25,&AF_S,b,RG_INK,selected?RG_PANEL:RG_BG);
        cv_text(218,y+6,&AF_S,"x",RG_WOOD);
    }
}
/* Sixteen large pads, one drum lane at a time. Cursor and transport are separate:
 * cream frame = edit, green underline = playing. Nothing is quantized or rewritten. */
static void reggae_step_pads(const track_t *t)
{
    unsigned base=ui.bank*16u,len=(unsigned)clamp(t->p[P_SLEN],1,NSTEP);
    int drum=drum_track(t);uint64_t locks=motion_lock_steps(trk_index(t));
    for(unsigned i=0;i<16;i++) {
        unsigned step=base+i;int x=6+(i%4)*58,y=3+(i/4)*30;
        int enabled=step<len,selected=step==ui.cursor;
        const step_t *st=&seq_steps(t)[step];
        int on=enabled && (drum?(st->time==ST_NOTE && (step_lanes(st)&(1u<<ui.lane))):step_on(st));
        uint16_t fill=on?RG_PANEL:RG_BG,edge=selected?RG_INK:enabled?RG_WOOD:RG_EDGE;
        char n[8],value[16];
        cv_rrect(x,y,54,26,-2,edge,RG_BG);cv_rect(x+2,y+2,50,22,fill);
        fmt_int(n,step+1);cv_text(x+4,y+6,&AF_S,n,enabled?RG_GOLD:RG_EDGE);
        str_cpy(value,"--",sizeof value);
        if(enabled) {
            if(drum)str_cpy(value,on?"HIT":"--",sizeof value);
            else if(st->time==ST_TIE)str_cpy(value,"HOLD",sizeof value);
            else if(st->time==ST_NOTE && st->n) {
                note_name(value,st->note[0]);
                if(st->n>1){unsigned end=str_len(value);value[end]='+';value[end+1]=0;}
            }
        }
        cv_text_in(x+20,y+6,31,&AF_S,value,on?RG_INK:RG_WOOD,fill);
        if(enabled && ((locks>>step)&1u))cv_rect(x+47,y+3,3,3,RG_GOLD);
        if(on && (drum?(step_accents(st)&(1u<<ui.lane)):(st->flags&SF_ACCENT)))cv_rect(x+3,y+3,3,3,RG_RED);
        if(enabled && song.playing && step==t->seq_idx)cv_rect(x+3,y+22,48,3,RG_GREEN);
        if(selected)cv_rect(x,y+8,2,10,RG_INK);
    }
}
static void reggae_seq_draw(void)
{
    static uint32_t last;
    int arrange=cur_page()->graph==GR_SONG;
    uint8_t mono=ux.mono,light=ux.light;
    char b[32],v[4][16];const char *label[4];
    if(!ui.force && (uint32_t)(fm1_ms-last)<100u)return;
    last=fm1_ms;cursor_fix();ux.mono=ux.light=0;ux.gen++;
    cv_begin(240,56,RG_BG);
    cv_rect(8,8,4,15,song.rec?RG_RED:song.playing?RG_GREEN:RG_GOLD);
    cv_text(19,7,&AF_M,arrange?"SONG":drum_track(TSEL)?"MAKE A BEAT":"MAKE A PATTERN",RG_INK);
    fmt_int(b,song.g[G_BPM]);cv_text_in(184,9,48,&AF_S,b,RG_GOLD,RG_BG);
    if(arrange) {
        for(unsigned k=0;k<NTRK;k++) { b[0]='T';b[1]='1'+k;b[2]=0;
            cv_text_in(37+k*37,35,31,&AF_S,b,RG_CONTROL_COLOR[k],RG_BG); }
        cv_text(191,35,&AF_S,"LOOPS",RG_WOOD);
        if(chain.running){fmt_int(b,chain.row+1);cv_text_in(7,35,25,&AF_S,b,RG_GREEN,RG_BG);}
    } else {
        b[0]='T';b[1]='1'+song.sel;b[2]=0;cv_text(9,34,&AF_S,b,RG_GOLD);
        int idx=reggae_index(TSEL);
        cv_text_in(36,34,195,&AF_S,drum_track(TSEL)?drum_lane_name(TSEL,ui.lane):idx>=0?ENGINES[TSEL->eng_req]->presets[TSEL->preset].name:ENGINES[TSEL->eng_req]->name,RG_INK,RG_BG);
    }
    cv_blit(0,0);
    cv_begin(240,124,RG_BG);cv_oy=0;
    if(arrange)reggae_arrangement();
    else reggae_step_pads(TSEL);
    cv_blit(0,56);
    cv_begin(240,60,RG_BG);
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
            label[1]="NOTE";label[2]=step_on(st)?"LENGTH":"TIME";label[3]="ACC/SLIDE";
            if(st->n)note_name(v[1],st->note[0]);else str_cpy(v[1],"--",16);
            if(step_on(st))fmt_int(v[2],note_len(TSEL,ui.cursor));
            else str_cpy(v[2],st->time==ST_TIE?"TIE":st->time==ST_REST?"REST":"NOTE",16);
            str_cpy(v[3],st->flags&SF_ACCENT?st->flags&SF_SLIDE?"ACC+SL":"ACCENT":st->flags&SF_SLIDE?"SLIDE":"NORMAL",16);
        }
    }
    for(unsigned k=0;k<4;k++)reggae_seq_card(k,label[k],v[k]);
    const char *hint=drum_track(TSEL)?"CHOOSE STEP / TURN HIT ON":"REC: ADD NOTES / PLAY: HEAR";
    if(arrange) {
        hint=chain.running?"PLAY: STOP / SONG RUNNING":"PLAY: SONG / SAVE: PROJECT";
        if(ui.song_row>=chain_config.count)hint="TURN PATTERN TO ADD A SECTION";
        else {unsigned sl=chain_config.row[ui.song_row].slot[ui.song_trk%NTRK];
            if(sl<4&&!graph_project_used(sl))hint="PATTERN NOT SAVED: USE SAVE";}
    }
    cv_text_in(2,47,236,&AF_S,ui.msg_t?ui.msg:hint,RG_INK,RG_BG);
    cv_blit(0,180);
    if(ui.msg_t)ui.msg_t--;if(ui.hot_t)ui.hot_t--;
    ux.mono=mono;ux.light=light;ux.gen++;ui.force=0;
}
