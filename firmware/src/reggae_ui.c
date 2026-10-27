/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS native 240x240 illustrations. Integer geometry, existing 124-row
 * canvas, no allocations/audio-ISR work. Visual snapshots limited to 10 Hz. */
#define RG_BG RGB(12,17,16)
#define RG_PANEL RGB(29,34,30)
#define RG_INK RGB(239,224,187)
#define RG_GOLD RGB(235,172,47)
#define RG_GREEN RGB(83,143,112)
#define RG_RED RGB(207,73,47)
#define RG_WOOD RGB(128,108,75)
#define RG_EDGE RGB(77,66,47)
#define RG_CONE RGB(43,52,47)

/* Coverage is blended into the existing strip, never a larger framebuffer.
 * Only circle boundary pixels use 4x4 samples; interiors are filled as spans. */
static void reggae_ink(int x,int y,uint16_t color,unsigned alpha)
{
    int yy=y+cv_oy;
    if(!alpha || (unsigned)x>=cv_w || (unsigned)yy>=cv_h)return;
    uint16_t under=swap16(cv_px[yy*cv_w+x]);
    cv_pset(x,y,alpha>=16?color:ux_mix(under,color,(int)(alpha*100/16)));
}
static void reggae_line(int x0,int y0,int x1,int y1,uint16_t color)
{
    int steep=(y1>y0?y1-y0:y0-y1)>(x1>x0?x1-x0:x0-x1),v;
    if(x0==x1 || y0==y1){cv_line(x0,y0,x1,y1,color);return;}
    if(steep){v=x0;x0=y0;y0=v;v=x1;x1=y1;y1=v;}
    if(x0>x1){v=x0;x0=x1;x1=v;v=y0;y0=y1;y1=v;}
    for(int x=x0;x<=x1;x++) {
        int q=y0*256+(y1-y0)*256*(x-x0)/(x1-x0),y=q>>8;
        unsigned a=((unsigned)q&255u)>>4;
        reggae_ink(steep?y:x,steep?x:y,color,16-a);
        reggae_ink(steep?y+1:x,steep?x:y+1,color,a);
    }
}
static const uint16_t RG_CONTROL_COLOR[]={RG_GOLD,RG_GREEN,RG_INK,RG_RED};
/* Musical labels for the curated bank; parameter descriptors retain exact units. */
static const char *const RG_CONTROL_NAME[][4]={
    {"HARM","TONE","ROTOR","DRIVE"}, {"HARM","TONE","ROTOR","DRIVE"},
    {"CUTOFF","RES","ENV","DRIVE"}, {"CUTOFF","RES","ENV","DRIVE"},
    {"ATTACK","TONE","VIBRATO","RELEASE"}, {"TIMBRE","ENV","CHORUS","DETUNE"},
    {"TONE","DECAY","DRIVE","RELEASE"}, {"TONE","ATTACK","DECAY","RELEASE"},
    {"BRIGHT","DAMP","PLUCK","RELEASE"}, {"PITCH","RATE","ECHO","TONE"},
    {"TIMBRE","ENV","RATIO","LEVEL"}, {"HARM","TONE","ROTOR","ECHO"},
    {"TONE","DECAY","SNAP","DRIVE"},
    {"FALL","LENGTH","ECHO","TONE"}, {"RANGE","RATE","ECHO","TONE"},
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
    if(r<1){cv_pset(x,y,col);return;}
    for(int dy=-r;dy<=r;dy++) {
        int inner=r-1,outer=r;
        while(inner>=0 && inner*inner+dy*dy>(r-1)*(r-1))inner--;
        while(outer>=0 && outer*outer+dy*dy>(r+1)*(r+1))outer--;
        if(inner>=0)cv_rect(x-inner,y+dy,inner*2+1,1,col);
        for(int dx=inner+1;dx<=outer;dx++) {
            unsigned a=0;
            for(int sy=-3;sy<=3;sy+=2)for(int sx=-3;sx<=3;sx+=2){
                int xx=dx*8+sx,yy=dy*8+sy;
                a+=xx*xx+yy*yy<=r*r*64;
            }
            reggae_ink(x+dx,y+dy,col,a);
            if(dx)reggae_ink(x-dx,y+dy,col,a);
        }
    }
}
static void reggae_ring(int x,int y,int r,uint16_t col,uint16_t bg)
{ reggae_disc(x,y,r,col); reggae_disc(x,y,r-2,bg); }
static void reggae_keys(int x,int y,int w,int h,int level)
{
    int k;
    for(k=0;k<14;k++) {
        int pressed=level>400 && k%4==0,top=y+pressed;
        cv_rect(x+k*w,top,w-1,h-pressed,pressed?RG_GOLD:RG_INK);
        cv_rect(x+k*w,top+h-pressed-2,w-1,2,RG_WOOD);
        if(k%7!=2 && k%7!=6) {
            cv_rrect(x+k*w+w-4,y,4,h*3/5,-1,RG_BG,RG_INK);
            cv_rect(x+k*w+w-3,y+1,2,2,RG_CONE);
        }
    }
}
static void reggae_speaker(int x,int y,int w,int r,int pulse)
{
    int cy=y+r+6,cx=x+w/2,h=r*2+12;
    cv_rrect(x+3,y+3,w,h,-3,RG_EDGE,RG_BG);
    cv_rrect(x,y,w,h,-3,RG_WOOD,RG_BG);
    cv_rrect(x+3,y+3,w-6,h-6,-2,RG_PANEL,RG_WOOD);
    reggae_disc(cx,cy,r,RG_INK);
    reggae_disc(cx,cy,r-2,RG_BG);
    reggae_disc(cx,cy,r-5+pulse,RG_CONE);
    reggae_ring(cx,cy,r-7+pulse,RG_GREEN,RG_CONE);
    reggae_disc(cx,cy,4+pulse,RG_INK);
    cv_rect(x+5,y+5,2,2,RG_INK);cv_rect(x+w-7,y+5,2,2,RG_INK);
    cv_rect(x+5,y+h-7,2,2,RG_INK);cv_rect(x+w-7,y+h-7,2,2,RG_INK);
}
/* A shared visual family: cream eyes, dark pupils and simple musical gestures. */
static void reggae_eye(int x,int y,int r,int dx,int dy)
{
    reggae_disc(x,y,r,RG_INK);
    reggae_disc(x+clamp(dx,-r/3,r/3),y+clamp(dy,-r/3,r/3),r/3,RG_BG);
}
static void reggae_mouth(int x,int y,int w,int opening,uint16_t body)
{
    cv_rrect(x,y,w,3+opening,-2,RG_BG,body);
    if(opening>3)cv_rect(x+3,y+opening,w-6,2,RG_RED);
}
static void reggae_art(track_t *t,int kind,int level,int motion)
{
    int k,ph=motion?(int)(t->lfo_ph>>22):0;
    int pulse=motion?clamp(level*18/32768,0,5):0;
    cv_rect(12,117,216,1,RG_INK);
    if(kind==RG_ORGAN) {
        cv_rrect(17,45,165,65,-5,RG_EDGE,RG_BG);
        cv_rrect(14,42,165,63,-5,RG_WOOD,RG_BG);
        cv_rrect(23,45,147,15,-3,RG_PANEL,RG_WOOD);
        cv_rrect(23,3,153,38,-4,RG_PANEL,RG_BG);
        for(k=0;k<9;k++) {
            int x=30+k*16,bar=drw_level(t->p,k);
            if(k==8 && t->p[P_E4])bar=0;
            cv_rect(x+3,7,3,28,RG_BG);
            for(int tick=0;tick<4;tick++)cv_rect(x+8,9+tick*7,2,1,RG_EDGE);
            cv_rrect(x-1,7+bar*3,11,6,-1,k<2?RG_RED:RG_INK,RG_PANEL);
            cv_rect(x+1,8+bar*3,7,1,k<2?RG_WOOD:RG_GOLD);
        }
        for(k=0;k<5;k++)cv_rect(30+k*11,50,7,3,k==0?RG_RED:RG_INK);
        reggae_keys(26,62,10,15,level);
        cv_rect(23,78,147,3,RG_EDGE);
        reggae_keys(26,83,10,17,level);
        cv_rect(19,104,155,4,RG_INK);
        cv_rect(25,108,6,9,RG_WOOD);cv_rect(162,108,6,9,RG_WOOD);
        cv_rrect(187,5,40,106,-3,RG_EDGE,RG_BG);
        cv_rrect(189,7,36,100,-2,RG_WOOD,RG_EDGE);
        reggae_speaker(187,64,40,17,pulse/3);
        { int rp=motion?(int)(drw_t[song.sel].rph[0]>>22):0;
          int dx=SINE[rp&1023]*13/32768,dy=SINE[(rp+256)&1023]*13/32768;
          reggae_disc(207,32,15,RG_BG);
          reggae_line(207-dx,32-dy,207+dx,32+dy,RG_INK);
          reggae_disc(207-dx,32-dy,3,RG_RED);
          reggae_disc(207+dx,32+dy,3,RG_GOLD);
          reggae_disc(207,32,3,RG_INK);
          for(k=0;k<3;k++)cv_rect(195,51+k*3,24,1,RG_EDGE); }
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
            reggae_line(16,30,knee-5,30,RG_GOLD);
            reggae_line(knee-5,30,knee,30-crest,RG_GREEN);
            reggae_line(knee,30-crest,72,52,RG_GOLD);
        }
    } else if(kind==RG_REED) {
        /* Bellows are a breath metaphor, not a model of the physical reed. */
        int breath=motion?clamp((level-200)*8/6000,0,8):0;
        int sway=motion?t->lfo_val*2/32768:0;
        cv_rrect(27,36,180,72,-7,RG_EDGE,RG_BG);
        cv_rrect(25,32,180,72,-7,RG_GREEN,RG_BG);
        cv_rrect(34,38,104,38,-3,RG_PANEL,RG_GREEN);
        for(k=0;k<10;k++) {
            int x=38+k*(8+breath/4);
            reggae_line(x,40,x+4,56,RG_INK);
            reggae_line(x+4,56,x,73,RG_WOOD);
        }
        reggae_eye(158,49,12,sway,breath/3);
        reggae_eye(187,49,12,sway,breath/3);
        reggae_mouth(164,69,21,breath,RG_GREEN);
        reggae_keys(33,83,9,18,level);
        cv_rect(169,91,23,3,RG_GOLD);
        reggae_line(45,105,40,113,RG_INK);reggae_line(40,113,54,113,RG_INK);
        reggae_line(183,105,188,113,RG_INK);reggae_line(188,113,174,113,RG_INK);
        if(breath)for(k=0;k<3;k++) {
            int y=66+k*8;
            reggae_line(210,y,216+breath,y+sway,RG_GREEN);
        }
    } else if(kind==RG_PIANO) {
        cv_rrect(25,38,190,65,-5,RG_WOOD,RG_BG);
        cv_rrect(29,42,182,57,-3,RG_PANEL,RG_WOOD);
        for(k=0;k<10;k++){int h=(k%3==0?pulse:0);cv_rect(39+k*17,14+h,3,24-h,RG_GREEN);cv_rect(35+k*17,12+h,10,5,RG_GOLD);}
        reggae_keys(35,67,12,27,level);
        cv_rect(33,49,174,2,RG_EDGE);
        for(k=0;k<4;k++)reggae_disc(43+k*13,57,3,k==0?RG_RED:RG_INK);
        cv_rect(33,104,6,13,RG_INK);cv_rect(201,104,6,13,RG_INK);
    } else if(kind==RG_BRASS) {
        cv_rrect(43,55,92,33,-8,RG_GOLD,RG_BG);
        cv_rrect(49,61,80,21,-6,RG_BG,RG_GOLD);
        cv_rect(36,49,102,12,RG_GOLD);cv_rect(36,49,102,2,RG_INK);
        for(k=0;k<3;k++){cv_rect(69+k*19,36-pulse,4,34,RG_INK);cv_rect(65+k*19,32-pulse,12,4,RG_GOLD);}
        for(k=0;k<42;k++)cv_rect(133+k,46-k/2,1,19+k,RG_GOLD);
        cv_rrect(171,24,7,66,-3,RG_INK,RG_BG);
        cv_rect(174,28,3,58,RG_EDGE);
        cv_rect(28,48,9,4,RG_INK);
        for(k=0;k<3;k++)if(level>500)reggae_line(186,55,199+pulse*2,30+k*25,RG_GREEN);
    } else if(kind==RG_STRING) {
        /* Horizontal acoustic silhouette: lower/upper bouts, waist, aligned
         * fingerboard, six strings, soundhole, bridge and three-a-side tuners. */
        reggae_disc(49,67,32,RG_WOOD);reggae_disc(81,67,24,RG_WOOD);
        reggae_disc(49,67,29,RG_GOLD);reggae_disc(81,67,21,RG_GOLD);
        reggae_disc(70,30,16,RG_BG);reggae_disc(70,104,16,RG_BG);
        cv_rect(89,59,106,17,RG_WOOD);cv_rect(90,61,105,13,RG_PANEL);
        cv_rrect(192,55,28,24,-4,RG_WOOD,RG_BG);
        reggae_ring(65,67,14,RG_INK,RG_GOLD);reggae_disc(65,67,11,RG_BG);
        cv_rrect(35,56,6,23,-2,RG_EDGE,RG_GOLD);
        for(k=0;k<10;k++)cv_rect(98+k*9,61,1,13,RG_WOOD);
        for(k=0;k<6;k++) {
            int y=62+k*2,vib=motion && k%2?pulse/2:0;
            reggae_line(39,y,67,y+vib,RG_INK);reggae_line(67,y+vib,213,y,RG_INK);
        }
        for(k=0;k<3;k++) {
            cv_rect(199+k*7,51,3,6,RG_INK);cv_rect(199+k*7,78,3,6,RG_INK);
        }
        cv_rect(191,59,3,17,RG_INK);
    } else if(kind==RG_SIREN) {
        int dy=motion?t->lfo_val*5/32768:0;
        cv_rrect(83,29,80,76,-7,RG_EDGE,RG_BG);
        cv_rrect(79,25,80,76,-7,RG_RED,RG_BG);
        reggae_eye(99,48,17,dy,0);reggae_eye(139,48,17,dy,0);
        reggae_mouth(97,76,43,4+pulse,RG_RED);
        for(k=0;k<5;k++)cv_rect(101+k*7,77,3,3,RG_INK);
        cv_rect(96,103,45,7,RG_PANEL);cv_rect(90,112,57,5,RG_INK);
        for(k=0;k<3;k++)if(level>300){reggae_line(62-k*10,34-k*5,55-k*10,65,RG_GOLD);reggae_line(55-k*10,65,62-k*10,96+k*5,RG_GOLD);}
        for(k=0;k<50;k+=4)cv_rect(170+k,62+SINE[(ph+k*20)&1023]*20/32768,3,3,RG_GREEN);
    } else {
        /* Each lane uses actual voice activity, not a decorative beat loop. */
        for(k=0;k<8;k++) {
            int x=36+(k%4)*56,y=24+(k/4)*57;
            int active=0,v;
            for(v=0;v<NVOICE;v++)if(t->v[v].active && drum_lane(t->v[v].note)==(uint32_t)k)active=1;
            reggae_ring(x,y,19+(motion&&active?2:0),active?RG_GOLD:RG_INK,RG_PANEL);
            reggae_eye(x-7,y-5,5,0,0);reggae_eye(x+7,y-5,5,0,0);
            reggae_mouth(x-6,y+5,12,motion&&active?6:0,RG_PANEL);
            cv_text_in(x-25,y+23,50,&AF_S,drum_lane_name(t,(uint32_t)k),RG_INK,RG_BG);
        }
    }
}
/* Effects are visual metaphors for actual amounts and activity. No new DSP.
 * Record cadence uses the real tempo/division delay length, including clamps. */
static void reggae_effect_art(int mode,int motion,int level)
{
    int k,phase=motion?(int)(fm1_ms/8)&1023:0;
    int pulse=motion?clamp(level*18/32768,0,5):0;
    if(mode==2) {
        int amp=motion?level*(5+song.g[G_RSIZE]/8)/32768:0;
        int bounce=SINE[phase]*amp/32768,model=song.g[G_RTYPE];
        cv_rrect(70,7+bounce,100,33,-9,RG_GREEN,RG_BG);
        reggae_eye(100,22+bounce,10,0,0);reggae_eye(140,22+bounce,10,0,0);
        if(model==1) {
            int py=45+bounce,height=12+(127-song.g[G_RDAMP])/8;
            for(k=45;k<=98;k+=2) {
                int x=120+SINE[((k-45)*62)&1023]*height/32768;
                reggae_line(120+SINE[((k-47)*62)&1023]*height/32768,py,x,k+bounce,RG_INK);py=k+bounce;
            }
        } else {
            /* Room/hall are architectural bodies; never labelled as spring. */
            for(k=0;k<(model==2?5:3);k++)cv_frame(79+k*8,44+k*5+bounce,82-k*16,53-k*7,RG_INK);
        }
        reggae_line(105,99+bounce,97,107,RG_GREEN);reggae_line(135,99+bounce,143,107,RG_GREEN);
        cv_text_in(0,111,240,&AF_S,model==1?"SPRING":model==2?"HALL":"ROOM",RG_INK,RG_BG);
    } else if(mode==1) {
        uint32_t ms=delay_samples()*1000u/FS;
        int turn=motion?(int)((fm1_ms%(ms?ms:1))*1024u/(ms?ms:1)):0;
        int spacing=12+(int)ms/60,count=1+song.g[G_DFDBK]/24;
        for(k=0;k<count;k++) {
            int x=105-k*spacing;if(x<12)break;
            reggae_ring(x,65,clamp(13-k*2,3,13),song.g[G_DCOLOR]>63?RG_GOLD:RG_GREEN,RG_BG);
        }
        reggae_disc(170,61,43,RG_PANEL);reggae_ring(170,61,38,RG_WOOD,RG_PANEL);
        reggae_ring(170,61,31,RG_WOOD,RG_PANEL);reggae_disc(170,61,24,RG_RED);
        reggae_eye(161,56,7,0,0);reggae_eye(180,56,7,0,0);
        reggae_mouth(164,69,13,pulse,RG_RED);
        {int dx=SINE[turn]*37/32768,dy=SINE[(turn+256)&1023]*37/32768;reggae_disc(170+dx,61+dy,3,RG_INK);}
        cv_text_in(0,111,240,&AF_S,"REPEATS / FEEDBACK",RG_GREEN,RG_BG);
    } else {
        static const char *const names[]={"DRIVE","CHORUS","ECHO","REVERB"};
        for(k=0;k<4;k++) {
            int x=12+k*57,amount=TSEL->p[P_DIST+k],energy=pulse*amount/127;
            uint16_t color=k==0?RG_GOLD:k==1?RG_GREEN:k==2?RG_RED:RG_INK;
            if(k==0) {
                cv_rrect(x,31,47,53,-5,RG_GOLD,RG_BG);
                reggae_eye(x+13,43,7,0,0);reggae_eye(x+34,43,7,0,0);
                cv_rrect(x+5,58,37,10+amount/16+energy,-3,RG_BG,RG_GOLD);
                for(int j=0;j<5;j++){cv_rect(x+8+j*6,59,3,3+amount/32,RG_INK);cv_rect(x+8+j*6,65+amount/16+energy,3,3,RG_INK);}
            } else if(k==1) {
                int sway=motion?SINE[phase]*amount/32768/32:0;
                cv_rrect(x,39+sway,24,39,-6,RG_GREEN,RG_BG);
                cv_rrect(x+24,39-sway,24,39,-6,RG_WOOD,RG_BG);
                reggae_eye(x+12,51+sway,8,0,0);reggae_eye(x+36,51-sway,8,0,0);
                reggae_mouth(x+7,66+sway,10,energy,RG_GREEN);
                reggae_mouth(x+31,66-sway,10,energy,RG_WOOD);
                reggae_line(x+18,81,x+30,81,RG_INK);
            } else if(k==2) {
                reggae_disc(x+24,59,24,RG_PANEL);reggae_ring(x+24,59,21,RG_WOOD,RG_PANEL);
                reggae_disc(x+24,59,16,RG_RED);
                reggae_eye(x+18,55,5,0,0);reggae_eye(x+31,55,5,0,0);
                reggae_mouth(x+18,65,13,energy,RG_RED);
                for(int j=0;j<amount/25;j++)cv_rect(x+4+j*8,28,4,3,RG_RED);
            } else {
                int y=33+energy;
                cv_rrect(x+4,y,40,22,-7,RG_GREEN,RG_BG);
                reggae_eye(x+15,y+10,6,0,0);reggae_eye(x+33,y+10,6,0,0);
                for(int j=0;j<5;j++) {
                    int yy=y+24+j*5;
                    reggae_line(x+9,yy,x+39,yy+3,RG_INK);reggae_line(x+39,yy+3,x+9,yy+5,RG_INK);
                }
            }
            cv_rect(x+2,91,44,3,RG_PANEL);cv_rect(x+2,91,amount*44/127,3,color);
            cv_text_in(x-3,103,54,&AF_S,names[k],RG_INK,RG_BG);
        }
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
    cv_text(21,fx?7:9,fx?&AF_M:&AF_S,fx?"DUB EFFECTS":REGGAE_BANK[idx].category,RG_INK);
    for(unsigned part=0;part<4;part++) {
        char digit[2]={(char)('1'+part),0}; int x=184+part*14;
        uint16_t color=part==song.sel?RG_GOLD:RG_EDGE;
        if ((song.rec>>part)&1u) color=rg_rec_wait && (now/250u)&1u?RG_EDGE:RG_RED;
        cv_text(x,8,&AF_S,digit,color);
        if(song.playing && trk[part].seq_idx%4u==0)cv_rect(x,22,7,2,color);
    }
    if(fx)str_cpy(name,cur_page()->title,sizeof name);
    else str_cpy(name,ENGINES[t->eng_req]->presets[t->preset].name,sizeof name);
    text_fit(val,sizeof val,name,fx?&AF_M:&AF_S,216);
    cv_text(12,fx?31:33,fx?&AF_M:&AF_S,val,RG_INK);cv_blit(0,0);
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
        reggae_ring(x+30,12,10,RG_CONTROL_COLOR[k],RG_BG);reggae_line(x+30,12,x+30+dx,12+dy,RG_INK);
        reggae_value(d,*vp,val,sizeof val);
        cv_text_in(x+1,24,58,&AF_S,reggae_control_name(idx,k,d),RG_CONTROL_COLOR[k],RG_BG);
        if(hot)cv_rect(x+4,1,3,3,RG_CONTROL_COLOR[k]);
        cv_text_in(x+1,36,58,&AF_S,val,RG_INK,RG_BG);
    }
    cv_text_in(0,48,240,&AF_S,rg_rec_wait?"READY: PLAY NOTE / PLAY TO START":
        rg_live_rec && song.rec?"RECORDING / REC: FINISH":
        ui.msg_t?ui.msg:fx?"FX: PAGE / HOME: PLAY":song.playing?"LOOP PLAYING / REC: LAYER":"REC: RECORD / ALGORITHM: TRACK",RG_INK,RG_BG);
    cv_blit(0,180);
    if(ui.hot_t)ui.hot_t--;if(ui.msg_t)ui.msg_t--;
    ux.mono=old_mono;ux.light=old_light;ux.gen++;ui.force=0;
}

/* Shared rack modules for the remaining technical editors. Keep their graphs,
 * units, key actions and native strip geometry; no extra framebuffer. */
static void reggae_rack_card(uint32_t c,const char *label,const char *value,const char *unit,int32_t ratio,int hot)
{
    char b[40],fitted[32];
    uint16_t color=hot?RG_INK:RG_CONTROL_COLOR[c%4];
    cv_begin(CARD_W,CARD_H,RG_BG);
    cv_rrect(0,0,CARD_W,CARD_H,3,RG_PANEL,RG_BG);
    reggae_ring(8,8,5,color,RG_PANEL);
    int ph=(640+clamp(ratio,0,1000)*762/1000)&1023;
    cv_line(8,8,8+SINE[ph]*3/32768,8-SINE[(ph+256)&1023]*3/32768,color);
    /* Label below the cap, full width for long DSP names. */
    text_fit(fitted,sizeof fitted,label,&AF_XS,CARD_W-6);
    cv_text_in(3,15,CARD_W-6,&AF_XS,fitted,RG_INK,RG_PANEL);
    str_cpy(b,value,sizeof b);
    if(unit && unit[0] && !str_eq(unit,label)) {
        unsigned n=str_len(b);str_cpy(b+n," ",sizeof b-n);
        n=str_len(b);str_cpy(b+n,unit,sizeof b-n);
    }
    text_fit(fitted,sizeof fitted,b,&AF_XS,CARD_W-6);
    cv_text_in(3,28,CARD_W-6,&AF_XS,fitted,color,RG_PANEL);
    if(ratio>=0)cv_rect(17,7,clamp(ratio,0,1000)*(CARD_W-22)/1000,2,color);
    cv_blit(CARD_X(c),Y_LABEL);
}
