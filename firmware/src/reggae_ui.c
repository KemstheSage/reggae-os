/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS native 240x240 illustrations. Integer geometry, existing 124-row
 * canvas, no allocations/audio-ISR work. Visual snapshots limited to 10 Hz. */
#define RG_BG RGB(12,17,16)
#define RG_PANEL RGB(29,34,30)
#define RG_INK RGB(239,224,187)
#define RG_GOLD RGB(235,172,47)
#define RG_GREEN RGB(83,143,112)
#define RG_RED RGB(207,73,47)
static const uint16_t RG_CONTROL_COLOR[]={RG_GOLD,RG_GREEN,RG_INK,RG_RED};
/* Musical labels for the curated bank; parameter descriptors retain exact units. */
static const char *const RG_CONTROL_NAME[][4]={
    {"HARM","TONE","ROTOR","DRIVE"}, {"HARM","TONE","ROTOR","DRIVE"},
    {"CUTOFF","RES","ENV","DRIVE"}, {"CUTOFF","RES","ENV","DRIVE"},
    {"ATTACK","TONE","VIBRATO","RELEASE"}, {"TIMBRE","ENV","CHORUS","DETUNE"},
    {"TONE","DECAY","DRIVE","RELEASE"}, {"TONE","ATTACK","ENV","RELEASE"},
    {"BRIGHT","DAMP","PLUCK","RELEASE"}, {"PITCH","RATE","ECHO","TONE"},
    {"TIMBRE","ENV","RATIO","LEVEL"}, {"HARM","TONE","ROTOR","ECHO"},
    {"TONE","DECAY","SNAP","DRIVE"},
};
static struct {
    uint32_t ms;
    int index;
    const page_t *page;
    uint8_t valid,knob,part,home;
} rg_focus;

static int reggae_fxscreen(void)
{
    return !ui.home && (str_eq(cur_page()->title,"FX") || str_eq(cur_page()->title,"DLY") || str_eq(cur_page()->title,"REVERB"));
}
static int reggae_screen(void)
{
    return (ui.home && ui_home_view == HV_SCOPE && reggae_patch(TSEL)) || reggae_fxscreen();
}
/* UI-only notification: no parameter or DSP changes. Physical encoders and
 * emulated panel events share this path. No encoder-click hardware assumed. */
static void reggae_focus_turn(uint32_t k)
{
    int16_t *v;
    const param_desc_t *d;
    rg_focus.valid=0;
    if(!reggae_screen())return;
    d=ui.home?home_param(k,&v):page_desc(cur_page(),k,&v);
    if(!d || !d->label || d->label[0]=='-')return;
    rg_focus.ms=fm1_ms;rg_focus.knob=k;rg_focus.part=song.sel;
    rg_focus.index=reggae_index(TSEL);rg_focus.page=cur_page();rg_focus.home=ui.home;
    rg_focus.valid=1;
}
static int reggae_focus_active(void)
{
    return rg_focus.valid && (uint32_t)(fm1_ms-rg_focus.ms)<1200u &&
        rg_focus.part==song.sel && rg_focus.index==reggae_index(TSEL) &&
        rg_focus.home==ui.home && rg_focus.page==cur_page();
}
static const char *reggae_control_name(int idx,unsigned k,const param_desc_t *d)
{ return ui.home && idx>=0?RG_CONTROL_NAME[idx][k]:d->label; }
static void reggae_value(const param_desc_t *d,int value,char *s,uint32_t len)
{
    const char *unit;
    param_format(d,value,s,&unit);
    if(*unit){uint32_t n=str_len(s);if(n<len)str_cpy(s+n,unit,len-n);}
}
static void reggae_focus_art(int idx)
{
    unsigned k=rg_focus.knob;int16_t *v;char value[32],number[2]={(char)('1'+k),0};
    const param_desc_t *d=ui.home?home_param(k,&v):page_desc(cur_page(),k,&v);
    int ratio=d->max>d->min?clamp((*v-d->min)*127/(d->max-d->min),0,127):0;
    uint16_t color=RG_CONTROL_COLOR[k];
    const char *unit;int numeric=1;
    param_format(d,*v,value,&unit);
    for(unsigned n=0;value[n];n++)if(!((value[n]>='0' && value[n]<='9') || value[n]=='.' || value[n]=='-' || value[n]=='+'))numeric=0;
    cv_rect(12,5,18,18,color);cv_text_in(12,7,18,&AF_S,number,RG_BG,color);
    cv_text(39,6,&AF_M,reggae_control_name(idx,k,d),color);
    cv_text_in(12,35,216,numeric && text_w(&AF_L,value)<=216?&AF_L:&AF_M,value,RG_INK,RG_BG);
    cv_text_in(12,65,216,&AF_S,unit,RG_INK,RG_BG);
    /* Segmented range, with a distinct origin for bipolar parameters. */
    for(int n=0;n<24;n++)cv_rect(12+n*9,84,6,9,n*127<=ratio*23?color:RG_PANEL);
    if(d->min<0 && d->max>0){int x=12+(-d->min)*207/(d->max-d->min);cv_rect(x,97,2,5,RG_INK);}
    cv_rect(12+ratio*207/127,77,3,5,RG_INK);
    cv_text_in(0,108,240,&AF_S,"RETURNS TO PLAY AUTOMATICALLY",RG_INK,RG_BG);
}
static void reggae_disc(int x,int y,int r,uint16_t col)
{
    int dy,dx;
    for(dy=-r;dy<=r;dy++) {
        for(dx=0;dx*dx+dy*dy<=r*r;dx++);
        cv_rect(x-dx+1,y+dy,2*dx-1,1,col);
    }
}
static void reggae_ring(int x,int y,int r,uint16_t col,uint16_t bg)
{ reggae_disc(x,y,r,col); reggae_disc(x,y,r-2,bg); }
static void reggae_keys(int x,int y,int w,int h,int level)
{
    int k;
    for(k=0;k<14;k++) {
        cv_rect(x+k*w,y,w-1,h,level>400 && k%4==0?RG_GOLD:RG_INK);
        if(k%7!=2 && k%7!=6) cv_rect(x+k*w+w-3,y,3,h*3/5,RG_BG);
    }
}
static void reggae_speaker(int x,int y,int w,int r,int pulse)
{
    cv_rect(x,y,w,r*2+12,RG_PANEL); cv_frame(x,y,w,r*2+12,RG_INK);
    reggae_ring(x+w/2,y+r+6,r,RG_INK,RG_BG);
    reggae_ring(x+w/2,y+r+6,r-5+pulse,RG_GREEN,RG_BG);
    reggae_disc(x+w/2,y+r+6,5+pulse,RG_GOLD);
}
static void reggae_art(track_t *t,int kind,int level,int motion)
{
    int k,ph=motion?(int)(t->lfo_ph>>22):0;
    int pulse=motion?level*7/32768:0;
    cv_rect(12,117,216,1,RG_INK);
    if(kind==RG_ORGAN) {
        for(k=0;k<9;k++) {
            int x=30+k*16,bar=drw_level(t->p,k);
            if(k==8 && t->p[P_E4])bar=0;
            cv_rect(x+3,6,3,42,RG_PANEL);
            cv_rect(x,6+bar*4,10,5,k<2?RG_RED:RG_INK);
        }
        cv_rect(19,53,163,7,RG_INK);
        reggae_keys(26,62,10,17,level);
        reggae_keys(26,83,10,19,level);
        cv_rect(22,105,159,5,RG_INK);
        cv_rect(28,110,5,7,RG_INK);cv_rect(170,110,5,7,RG_INK);
        reggae_speaker(187,68,40,15,pulse/3);
        { int rp=motion?(int)(drw_t[song.sel].rph[0]>>22):0;
          int dx=SINE[rp&1023]*13/32768,dy=SINE[(rp+256)&1023]*13/32768;
          reggae_disc(207,35,19,RG_RED);
          cv_line(207-dx,35-dy,207+dx,35+dy,RG_INK); }
    } else if(kind==RG_BASS) {
        reggae_speaker(74,44,94,30,pulse/2);
        reggae_speaker(87,4,66,16,pulse/3);
        reggae_speaker(24,73,43,15,pulse/3);
        reggae_speaker(175,69,43,17,pulse/3);
        for(k=0;k<9;k++)cv_rect(223,108-k*9,5,5,level>k*2800?RG_GOLD:RG_PANEL);
        if(t->eng_req==0) {
            /* Schematic filter aperture: cutoff moves the knee; resonance
             * raises its crest. This is not a measured frequency response. */
            int knee=23+t->p[P_E4]*40/127,crest=t->p[P_E5]*14/127;
            cv_line(16,30,knee-5,30,RG_GOLD);
            cv_line(knee-5,30,knee,30-crest,RG_GREEN);
            cv_line(knee,30-crest,72,52,RG_GOLD);
        }
    } else if(kind==RG_REED) {
        /* A compact reed player; airflow follows envelope, wave phase follows vibrato. */
        reggae_disc(179,35,18,RG_INK);
        reggae_disc(179,26,19,RG_RED);cv_rect(159,29,40,6,RG_RED);
        for(k=0;k<5;k++)cv_line(165+k*6,11,162+k*8,28,RG_BG);
        cv_rect(174,37,3,3,RG_BG);cv_rect(185,37,3,3,RG_BG);
        cv_rect(166,55,31,49,RG_PANEL);cv_line(166,62,137,85,RG_INK);
        reggae_keys(29,82,9,22,level);
        cv_frame(24,77,136,31,RG_INK);
        for(k=0;k<3;k++) { int x,py=39+k*11;
            for(x=20;x<145;x+=5) { int y=39+k*11+SINE[(ph+x*8)&1023]*(2+t->p[P_LD_PIT]/4)/32768;
                if(level>300)cv_line(x-5,py,x,y,RG_GREEN);py=y; }
        }
    } else if(kind==RG_PIANO) {
        cv_rect(25,38,190,65,RG_PANEL);cv_frame(25,38,190,65,RG_INK);
        for(k=0;k<10;k++){int h=(k%3==0?pulse:0);cv_rect(39+k*17,14+h,3,24-h,RG_GREEN);cv_rect(35+k*17,12+h,10,5,RG_GOLD);}
        reggae_keys(35,67,12,27,level);
        cv_rect(33,104,6,13,RG_INK);cv_rect(201,104,6,13,RG_INK);
    } else if(kind==RG_BRASS) {
        cv_rect(36,49,102,12,RG_GOLD);cv_frame(45,61,87,25,RG_INK);
        for(k=0;k<3;k++){cv_rect(69+k*19,36-pulse,4,34,RG_INK);cv_rect(65+k*19,32-pulse,12,4,RG_GOLD);}
        for(k=0;k<42;k++)cv_rect(133+k,46-k/2,1,19+k,RG_GOLD);
        cv_rect(174,24,4,66,RG_RED);
        for(k=0;k<3;k++)if(level>500)cv_line(186,55,199+pulse*2,30+k*25,RG_GREEN);
    } else if(kind==RG_STRING) {
        reggae_disc(67,77,29,RG_GOLD);reggae_disc(91,60,22,RG_GOLD);
        cv_rect(90,48,113,15,RG_INK);cv_rect(193,43,24,25,RG_GREEN);
        reggae_disc(77,68,11,RG_BG);
        for(k=0;k<5;k++) {
            int y=50+k*3; cv_line(42,79+k,85, y+(motion?pulse:0),RG_BG);
            cv_line(85,y+(motion?pulse:0),211,y,RG_BG);
        }
        for(k=0;k<8;k++)cv_rect(108+k*10,48,1,15,RG_PANEL);
    } else if(kind==RG_SIREN) {
        int dy=motion?t->lfo_val*15/32768:0;
        cv_rect(84,34,65,65,RG_RED);cv_frame(84,34,65,65,RG_INK);
        reggae_ring(116,66,22,RG_INK,RG_BG);reggae_disc(116,66+dy/3,7,RG_GOLD);
        cv_rect(96,101,41,9,RG_PANEL);cv_rect(91,111,51,6,RG_INK);
        for(k=0;k<3;k++)if(level>300){cv_line(62-k*10,34-k*5,55-k*10,65,RG_GOLD);cv_line(55-k*10,65,62-k*10,96+k*5,RG_GOLD);}
        for(k=0;k<50;k+=4)cv_rect(168+k,62+SINE[(ph+k*20)&1023]*20/32768,3,3,RG_GREEN);
    } else {
        /* Each lane uses actual voice activity, not a decorative beat loop. */
        for(k=0;k<8;k++) {
            int x=36+(k%4)*56,y=24+(k/4)*57;
            int active=0,v;
            for(v=0;v<NVOICE;v++)if(t->v[v].active && drum_lane(t->v[v].note)==(uint32_t)k)active=1;
            reggae_ring(x,y,19+(motion&&active?2:0),active?RG_GOLD:RG_INK,RG_PANEL);
            cv_line(x-12,y-14,x+10,y+(active?4:-5),RG_RED);
            cv_text_in(x-25,y+23,50,&AF_S,drum_lane_name(t,(uint32_t)k),RG_INK,RG_BG);
        }
    }
}
static void reggae_effect_art(int mode,int motion,int level)
{
    int k,phase=motion?(int)(fm1_ms/8)&1023:0;
    if(mode==2) {
        int amp=motion?level*(5+song.g[G_RSIZE]/8)/32768:0,py=62;
        int height=5+(127-song.g[G_RDAMP])/10;
        cv_rect(18,40,5,42,RG_INK);cv_rect(217,40,5,42,RG_INK);
        for(k=23;k<218;k+=3){int y=62+SINE[((k-23)*32)&1023]*height/32768 + SINE[phase]*amp/32768;cv_line(k-3,py,k,y,RG_INK);py=y;}
        cv_text_in(0,101,240,&AF_S,"DECAY / DAMPING",RG_GREEN,RG_BG);
    } else if(mode==1) {
        int spacing=13+song.g[G_DTIME]*2;
        for(k=0;k<4;k++) {int x=22+k*spacing;if(x>115)break;reggae_disc(x,65,3+k*3,RG_INK);}
        reggae_disc(166,61,43,RG_RED);reggae_ring(166,61,33,RG_INK,RG_BG);
        reggae_disc(166,61,12,RG_GOLD);
        {int dx=SINE[phase]*36/32768,dy=SINE[(phase+256)&1023]*36/32768;cv_line(166,61,166+dx,61+dy,RG_INK);}
        for(k=0;k<song.g[G_DFDBK]/20;k++)cv_rect(118+k*15,111,9,3,RG_GREEN);
    } else {
        static const char *const names[]={"DRIVE","CHORUS","ECHO","REVERB"};
        for(k=0;k<4;k++){int x=12+k*57,h=TSEL->p[P_DIST+k]*64/127;
            cv_frame(x,16,48,76,RG_INK);cv_rect(x+6,85-h,36,h+1,k==2?RG_RED:k==3?RG_GREEN:RG_GOLD);
            cv_text_in(x-3,99,54,&AF_S,names[k],RG_INK,RG_BG);}
    }
}
static void reggae_draw(void)
{
    static uint32_t last; static int32_t envelope;
    track_t *t=TSEL;
    int idx=reggae_index(t),fx=reggae_fxscreen(),kind=idx>=0?REGGAE_BANK[idx].kind:RG_BASS;
    uint8_t old_mono=ux.mono,old_light=ux.light;
    uint32_t now=fm1_ms,k;
    int motion=!(ui_prefs&PREF_ANIM_OFF),mode=0,focus=reggae_focus_active();
    char name[32],val[24];
    if(!ui.force && (uint32_t)(now-last)<100u)return;
    last=now;ux.mono=ux.light=0;ux.gen++;
    {int pk=clamp(t->peak,0,32767);t->peak=0;envelope=pk>envelope?pk:envelope*3/4;}
    cv_begin(240,56,RG_BG);
    cv_rect(10,8,4,14,RG_RED);
    cv_text(21,7,&AF_M,fx?"DUB EFFECTS":REGGAE_BANK[idx].category,RG_INK);
    cv_rect(207,12,5,5,RG_GOLD);cv_rect(216,12,5,5,RG_GREEN);cv_rect(225,12,5,5,RG_PANEL);
    if(fx)str_cpy(name,cur_page()->title,sizeof name);
    else str_cpy(name,ENGINES[t->eng_req]->presets[t->preset].name,sizeof name);
    cv_text(12,31,&AF_M,name,RG_INK);cv_blit(0,0);
    cv_begin(240,124,RG_BG);
    if(focus)reggae_focus_art(idx);
    else if(fx){mode=str_eq(name,"DLY")?1:str_eq(name,"REVERB")?2:0;reggae_effect_art(mode,motion,envelope);}
    else reggae_art(t,kind,envelope,motion);
    cv_blit(0,56);
    cv_begin(240,60,RG_BG);
    for(k=0;k<4;k++) {
        int16_t *vp;const param_desc_t *d=fx?page_desc(cur_page(),k,&vp):home_param(k,&vp);
        int x=k*60,hot=ui.hot_t && ui.hot_col==k,ratio,ph,dx,dy;
        if(!d || !d->label || d->label[0]=='-')continue;
        ratio=d->max>d->min?clamp((*vp-d->min)*127/(d->max-d->min),0,127):0;
        ph=(640+ratio*6)&1023;dx=SINE[ph]*8/32768;dy=-SINE[(ph+256)&1023]*8/32768;
        reggae_ring(x+30,12,10,RG_CONTROL_COLOR[k],RG_BG);cv_line(x+30,12,x+30+dx,12+dy,RG_INK);
        reggae_value(d,*vp,val,sizeof val);
        cv_text_in(x+1,24,58,&AF_S,reggae_control_name(idx,k,d),RG_CONTROL_COLOR[k],RG_BG);
        if(hot)cv_rect(x+4,1,3,3,RG_CONTROL_COLOR[k]);
        cv_text_in(x+1,36,58,&AF_S,val,RG_INK,RG_BG);
    }
    cv_text_in(0,48,240,&AF_S,ui.msg_t?ui.msg:fx?"FX: PAGE / HOME: PLAY":"PRESETS: SOUND / EDIT",RG_INK,RG_BG);
    cv_blit(0,180);
    if(ui.hot_t)ui.hot_t--;if(ui.msg_t)ui.msg_t--;
    ux.mono=old_mono;ux.light=old_light;ux.gen++;ui.force=0;
}
