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

/* Four original speaker/fader companions: activity is pre-mixer voice activity,
 * and each fader is the actual track level. Mute/solo keeps existing key mapping. */
static void reggae_live_panel(int mixer)
{
    if(mixer)for(unsigned k=0;k<NTRK;k++) {
        track_t *t=&trk[k];int x=16+k*52,on=glo_sounding(k),active=0;
        for(unsigned v=0;v<NVOICE;v++)active|=t->v[v].active;
        int idx=reggae_index(t);char name[12];
        const char *cat=idx>=0?REGGAE_BANK[idx].category:ENGINES[t->eng_req]->name;
        text_fit(name,sizeof name,cat,&AF_XS,46);
        cv_text_in(x,1,48,&AF_XS,name,on?RG_CONTROL_COLOR[k]:RG_EDGE,RG_BG);
        cv_rrect(x+8,17,32,28,4,on?RG_PANEL:RG_BG,RG_BG);
        cv_frame(x+8,17,32,28,on?RG_CONTROL_COLOR[k]:RG_EDGE);
        for(int side=-1;side<=1;side+=2) {
            if(on)reggae_disc(x+24+side*7,26,3,RG_INK);
            else cv_rect(x+22+side*7,26,5,1,RG_EDGE);
        }
        cv_rect(x+20,35,9,on&&active?5:1,on?RG_GOLD:RG_EDGE);
        /* Volume faders stay visible in every knob mode. Above: pan position;
         * beside each fader: the reverb send, distinct from audio metering. */
        cv_rect(x+8,51,32,1,RG_EDGE);cv_rect(x+23,48,1,7,RG_EDGE);
        reggae_disc(x+8+(clamp(t->p[P_PAN],-64,63)+64)*31/127,51,2,on?RG_INK:RG_EDGE);
        cv_rect(x+23,60,3,38,RG_EDGE);
        int h=clamp(t->p[P_LEVEL],0,127)*34/127;
        cv_rect(x+18,95-h,14,5,on?RG_CONTROL_COLOR[k]:RG_EDGE);
        cv_rect(x+38,60,3,40,RG_EDGE);
        int wet=clamp(t->p[P_REV],0,127)*40/127;
        cv_rect(x+38,100-wet,3,wet,on?RG_GREEN:RG_EDGE);
        char label[8]={'T',(char)('1'+k),0};
        cv_text_in(x,104,48,&AF_XS,!on?"MUTED":perf_solo?"SOLO":label,on?RG_INK:RG_RED,RG_BG);
    } else {
        uint32_t held=perf_act,available=perf_avail();
        for(unsigned p=0;p<PF_KEYS;p++) {
            unsigned fx=perf_map[p];int x=16+(p%4)*52,y=3+(p/4)*23;
            int on=fx<PF_NFX&&((held>>fx)&1u),ok=fx<PF_NFX&&((available>>fx)&1u);
            uint16_t bg=on?RG_GOLD:RG_PANEL;char key[4]={W_NOTE[p],(char)('3'+(p>=4)+(p>=11)),0};
            cv_rrect(x,y,48,21,2,bg,RG_BG);cv_text(x+3,y,&AF_XS,key,on?RG_BG:RG_GOLD);
            cv_text_in(x+1,y+10,46,&AF_XS,fx<PF_NFX?PF_CELL[fx]:"--",!ok?RG_EDGE:on?RG_BG:RG_INK,bg);
        }
        for(unsigned k=0;k<NTRK;k++) {
            char label[8]={'M','U','T','E',' ','1'+k,0};int x=16+k*52;
            cv_text_in(x,104,48,&AF_XS,label,(held&PF_BIT(PF_M1+k))?RG_RED:RG_INK,RG_BG);
        }
    }
}
