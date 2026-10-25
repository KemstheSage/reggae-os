/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS pattern / arrangement workspace. Existing sequencer, project sources,
 * input mappings and persistence; 240x124 scratch strip, no new audio state. */
static int reggae_seq_screen(void)
{
    return !ui.home && (cur_page()->graph == GR_SONG ||
        (cur_page()->graph == GR_ROLL && !detail_on()));
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
    int cy=(small?18:25)-(motion&&sing?1:0),r=small?15:22;
    int eye=small?4:6,ey=cy-(small?2:3),blink=motion&&!sing&&fm1_ms%4200u<160u;
    int gaze=motion&&song.playing?(int)((TSEL->seq_idx/4u)%3u)-1:0;
    reggae_disc(26,cy,r,RG_WOOD);reggae_disc(26,cy,r-2,RG_BG);
    reggae_ring(26,cy,r-4,RG_EDGE,RG_BG);reggae_disc(26,cy,small?10:15,RG_GREEN);
    int x=26+SINE[ph]*(r-3)/32768,y=cy-SINE[(ph+256)&1023]*(r-3)/32768;
    reggae_disc(x,y,1,RG_GOLD);
    for(int side=-1;side<=1;side+=2) {
        int ex=26+side*eye;
        if(blink)cv_rect(ex-2,ey,5,1,RG_INK);
        else {reggae_disc(ex,ey,small?3:4,RG_INK);reggae_disc(ex+gaze,ey,1,RG_BG);}
    }
    /* Held notes and release tails keep the mouth open; silence closes it. */
    if(sing) {
        cv_rrect(small?23:21,cy+3,small?7:11,small?5:8,2,RG_BG,RG_GREEN);
        cv_rect(small?25:24,cy+(small?6:8),small?3:5,2,RG_RED);
    } else {
        cv_line(22,cy+4,24,cy+6,RG_BG);cv_line(24,cy+6,29,cy+6,RG_BG);cv_line(29,cy+6,31,cy+4,RG_BG);
    }
    if(!small) {
        /* Tiny hands lift on sounding notes; feet anchor it above the editor. */
        cv_rect(1,sing&&motion?26:34,4,3,RG_INK);cv_rect(48,sing&&motion?26:34,4,3,RG_INK);
        cv_rect(15,48,8,2,RG_GOLD);cv_rect(31,48,8,2,RG_RED);
    }
}
static void reggae_seq_card(unsigned k,const char *label,const char *value)
{
    int x=(int)k*60;
    cv_rrect(x+3,3,54,40,3,RG_PANEL,RG_BG);
    cv_rect(x+5,3,50,2,RG_CONTROL_COLOR[k]);
    cv_text_in(x+2,10,56,&AF_S,label,RG_CONTROL_COLOR[k],RG_PANEL);
    cv_text_in(x+2,25,56,&AF_M,value,RG_INK,RG_PANEL);
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
static void reggae_seq_draw(void)
{
    static uint32_t last;
    int arrange=cur_page()->graph==GR_SONG;
    __typeof__(ux) saved=ux;
    char b[32],v[4][16];const char *label[4];
    if(!ui.force && (uint32_t)(fm1_ms-last)<100u)return;
    last=fm1_ms;cursor_fix();ux.mono=ux.light=0;ux.gen++;
    ux.bg=ux.surf=RG_BG;ux.text=RG_INK;ux.accent=RG_RED;
    ux.mid=RG_GREEN;ux.dim=RG_EDGE;ux.raise=RG_WOOD;
    ux.lane=RG_PANEL;ux.grid=RGB(39,48,39);
    cv_begin(240,56,RG_BG);
    reggae_seq_record(arrange);
    cv_text(55,5,&AF_M,arrange?"DUB ARRANGE":drum_track(TSEL)?"RIDDIM GRID":"GROOVE ROLL",RG_INK);
    fmt_int(b,song.g[G_BPM]);cv_text(55,22,&AF_S,b,RG_GOLD);
    cv_text(82,22,&AF_S,"BPM",RG_WOOD);
    cv_text(116,22,&AF_S,song.rec?"REC":song.playing?"PLAY":"STOP",song.rec?RG_RED:RG_GREEN);
    for(unsigned beat=0;beat<4;beat++) {
        int active=song.playing && (TSEL->seq_idx/4u)%4u==beat;
        cv_rect(178+beat*14,24,10,3,active?RG_GOLD:RG_EDGE);
    }
    if(arrange) {
        for(unsigned k=0;k<NTRK;k++) { b[0]='T';b[1]='1'+k;b[2]=0;
            cv_text_in(37+k*37,35,31,&AF_S,b,RG_CONTROL_COLOR[k],RG_BG); }
        cv_text(191,35,&AF_S,"LOOPS",RG_WOOD);
        if(chain.running){fmt_int(b,chain.row+1);cv_text_in(7,35,25,&AF_S,b,RG_GREEN,RG_BG);}
    } else {
        b[0]='T';b[1]='1'+song.sel;b[2]=0;cv_text(55,37,&AF_S,b,RG_GOLD);
        int idx=reggae_index(TSEL);
        cv_text_in(78,37,157,&AF_S,idx>=0?ENGINES[TSEL->eng_req]->presets[TSEL->preset].name:ENGINES[TSEL->eng_req]->name,RG_INK,RG_BG);
    }
    cv_blit(0,0);
    cv_begin(240,124,RG_BG);cv_oy=0;
    cv_rect(4,0,232,1,RG_WOOD);cv_rect(4,123,232,1,RG_WOOD);
    if(arrange)reggae_arrangement();
    else { /* Reuse the established editable roll/grid, including ties, locks and playhead. */
        if(drum_track(TSEL))graph_grid(TSEL,RG_GOLD);else {pr_follow(TSEL);graph_roll(TSEL,RG_GOLD);}
    }
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
            label[1]="NOTE";label[2]=step_on(st)?"LENGTH":"TIME";label[3]="FEEL";
            if(st->n)note_name(v[1],st->note[0]);else str_cpy(v[1],"--",16);
            if(step_on(st))fmt_int(v[2],note_len(TSEL,ui.cursor));
            else str_cpy(v[2],st->time==ST_TIE?"TIE":st->time==ST_REST?"REST":"NOTE",16);
            str_cpy(v[3],st->flags&SF_ACCENT?st->flags&SF_SLIDE?"ACC+SL":"ACCENT":st->flags&SF_SLIDE?"SLIDE":"NORMAL",16);
        }
    }
    for(unsigned k=0;k<4;k++)reggae_seq_card(k,label[k],v[k]);
    const char *hint=drum_track(TSEL)?"SEQ: DETAIL / GLO: ARRANGE":"EDIT+SEQ HOLD: CLEAR ROLL";
    if(arrange) {
        hint=chain.running?"PLAY: STOP / SONG RUNNING":"PLAY: SONG / SAVE: PROJECT";
        if(ui.song_row>=chain_config.count)hint="KNOB 3: ADD / SAVE: PATTERNS";
        else {unsigned sl=chain_config.row[ui.song_row].slot[ui.song_trk%NTRK];
            if(sl<4&&!graph_project_used(sl))hint="PATTERN NOT SAVED: USE SAVE";}
    }
    cv_text_in(2,47,236,&AF_S,ui.msg_t?ui.msg:hint,RG_INK,RG_BG);
    cv_blit(0,180);
    if(ui.msg_t)ui.msg_t--;if(ui.hot_t)ui.hot_t--;
    saved.gen+=2;ux=saved;ui.force=0;
}
