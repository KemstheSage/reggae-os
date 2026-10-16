/* SPDX-License-Identifier: GPL-3.0-only
 * Exercise the actual emulated panel, audio ISR, UI and NOR-backed preset path.
 * Each invocation starts with fresh DSP state; run.sh compares knob variants. */
#include "../web/emu/felucca_web.c"
#include <assert.h>
static void advance(unsigned ms) { for (unsigned n=0; n<ms*441/10; n+=128) web_render(128); }
static void picture(const char *path) {
    FILE *f=fopen(path,"wb"); assert(f);
    fprintf(f,"P6\n240 240\n255\n");
    for (unsigned i=0;i<240*240;i++) {
        unsigned p=web_fb[i], c=(p>>8)|((p&255)<<8);
        unsigned char rgb[3]={(c>>11)*255/31,((c>>5)&63)*255/63,(c&31)*255/31};
        fwrite(rgb,1,3,f);
    }
    fclose(f);
}
static void prefix_unchanged(const engine_t *a, const engine_t *b) {
    for(unsigned i=0;i<a->npresets;i++) {
        const preset_t *x=&a->presets[i], *y=&b->presets[i];
        assert(!strcmp(x->name,y->name)); assert(!memcmp(x->e,y->e,sizeof x->e));
        assert(!memcmp(x->env,y->env,sizeof x->env)); assert(!memcmp(x->fx,y->fx,sizeof x->fx));
        assert(x->fenv==y->fenv && x->mono==y->mono && x->pat==y->pat);
    }
}
#ifndef REGGAE_NO_MAIN
int main(int argc,char **argv) {
    prefix_unchanged(&UPSTREAM_ENG_ANALOG,&ENG_ANALOG);
    prefix_unchanged(&UPSTREAM_ENG_WHEEL,&ENG_WHEEL);
    prefix_unchanged(&UPSTREAM_ENG_SAMPLE,&ENG_SAMPLE);
    assert(argc>=4);
    unsigned preset=atoi(argv[1]); int knob=atoi(argv[2]);
    assert(preset<NELEM(REGGAE_BANK) && knob>=-1 && knob<4);
    web_nor_erase(); web_boot(); advance(1000);
    for(unsigned i=0;i<preset;i++){ web_enc(EN_PRESET,1); advance(100); }
    assert(TSEL->eng_req==REGGAE_BANK[preset].engine);
    assert(TSEL->preset==REGGAE_BANK[preset].preset);
    int knob_delta=0;
    int16_t before[P_COUNT]; memcpy(before,TSEL->p,sizeof before);
    if(knob>=0) {
        int16_t *v; const param_desc_t *d=home_param(knob,&v);
        int delta=preset<4?d->max-*v:(d->max>*v?(d->max-*v>16?16:d->max-*v):(d->min-*v < -16?-16:d->min-*v));
        knob_delta=delta;
        web_enc(EN_K1+knob, delta); advance(200);
        assert(*v!=before[v-TSEL->p]);
    }
    if(knob<0) advance(200); /* identical note timing for every knob comparison */
    FILE *f=fopen(argv[3],"wb"); assert(f);
    unsigned blocks=44100*4/128; wav_hdr(f,blocks*128);
    double energy=0,tail=0; float peak=0; uint64_t hash=1469598103934665603ULL;
    /* organ triad; bass at C2. Same MIDI path as external notes. */
    int kind=REGGAE_BANK[preset].kind;
    int note=(kind==RG_BASS || kind==RG_DRUM)?36:60;
    int chord=kind==RG_ORGAN || kind==RG_PIANO || kind==RG_BRASS || kind==RG_STRING;
    web_midi(0x90,note,100);
    if(chord){web_midi(0x90,note+3,100);web_midi(0x90,note+7,100);}
    for(unsigned b=0;b<blocks;b++) {
        if(b==344) {web_midi(0x80,note,0);web_midi(0x80,note+3,0);web_midi(0x80,note+7,0);}
        web_render(128);
        if(b==170 && argc>4) picture(argv[4]);
        for(unsigned i=0;i<128;i++) {
            float l=out_l[i],r=out_r[i]; assert(isfinite(l)&&isfinite(r));
            if(fabsf(l)>peak) peak=fabsf(l); if(fabsf(r)>peak) peak=fabsf(r);
            energy+=l*l+r*r; if(b>blocks-35)tail+=l*l+r*r;
            int32_t li=(int32_t)(l*32768),ri=(int32_t)(r*32768);
            hash=(hash^(uint32_t)li)*1099511628211ULL;
            wav_put(f,li,ri);
        }
    }
    fclose(f);
    assert(energy>0.1 && peak>0.005 && peak<0.99);
    if(preset<4 || knob<0) assert(tail<0.02);
    /* Repeated chords exercise percussion retrigger and release/voice allocation. */
    for(unsigned j=0;j<24;j++) {
        web_midi(0x90,note,100); web_midi(0x90,note+4,100); web_midi(0x90,note+7,100); advance(35);
        web_midi(0x80,note,0); web_midi(0x80,note+4,0); web_midi(0x80,note+7,0); advance(35);
    }
    advance(preset<4?1500:10000);
    for(unsigned p=0;p<NPART;p++) for(unsigned v=0;v<NVOICE;v++) assert(!trk[p].v[v].active);
    /* Save/recall real parameter values through the original preset storage. */
    song.playing=0;
    up_ui_named(2,0,"RG TEST",preset<2?CAT_KEYS:CAT_BASS);
    assert(up_used(0));
    uint8_t fm_saved[FP_SIZE+1]; memcpy(fm_saved,fm6_patch[song.sel],sizeof fm_saved);
    int16_t saved[P_COUNT]; memcpy(saved,TSEL->p,sizeof saved);
    TSEL->p[P_E6]=0; assert(up_load(0)==0);
    assert(!memcmp(saved,TSEL->p,sizeof saved));
    if(TSEL->eng_req==12) assert(!memcmp(fm_saved,fm6_patch[song.sel],FP_SIZE));
    /* The dedicated upstream emulator test additionally verifies SAVE button gestures. */
    printf("preset=%u knob=%d delta=%d note=%d chord=%d peak=%.5f energy=%.5f tail=%.9f hash=%llu\n",preset,knob,knob_delta,note,chord,peak,energy,tail,(unsigned long long)hash);
    return 0;
}

#endif
