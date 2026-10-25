/* SPDX-License-Identifier: GPL-3.0-only */
static int reggae_seq_euclid(void) { return eu.on; }
/* The held SEQ tool keyboard uses the existing actions and note mapping. */
static void reggae_seq_tools(void)
{
    if(eu.on) {
        cv_text_in(16,2,204,&AF_S,"RHYTHM PREVIEW",RG_INK,RG_BG);
        for(unsigned i=0;i<NSTEP;i++) {
            int x=17+(i%16)*12,y=25+(i/16)*17;
            cv_rect(x,y,9,11,i>=eu.steps?RG_EDGE:eu_hit(i)?RG_GOLD:RG_PANEL);
        }
    } else for(unsigned p=0;p<TL_N;p++) {
        if(!tl_cell(TSEL,p))continue;
        int x=16+(p%4)*52,y=3+(p/4)*23;
        char key[4]={W_NOTE[p],(char)('3'+(p>=4)+(p>=11)),0};
        cv_rrect(x,y,48,21,2,RG_PANEL,RG_BG);
        cv_text(x+3,y,&AF_XS,key,RG_GOLD);
        const char *name=p==TL_LEFT?"SHIFT <":p==TL_RIGHT?"SHIFT >":p==TL_DOUBLE?"DOUBLE":p==TL_HALF?"HALF":TL_NAME[p];
        cv_text_in(x+1,y+10,46,&AF_XS,name,RG_INK,RG_PANEL);
    }
    if(drum_track(TSEL))for(unsigned p=0;p<NLANE;p++) {
        int x=16+p*26;uint16_t bg=p==ui.lane?RG_GOLD:RG_PANEL;
        cv_rect(x,102,24,17,bg);
        cv_text_in(x,102+CAP_IN(XS,17),24,&AF_XS,drum_lane_abbr(TSEL,p),p==ui.lane?RG_BG:RG_INK,bg);
    }
}
