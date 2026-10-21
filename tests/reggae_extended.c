/* SPDX-License-Identifier: GPL-3.0-only
 * Original audition phrases and the eight ROOTS KIT lanes through firmware MIDI.
 * Does not change the approved organ audition generator. */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(int argc,char **argv) {
    assert(argc==3); unsigned preset=atoi(argv[1]); assert(preset>=4 && preset<NELEM(REGGAE_BANK));
    web_nor_erase();web_boot();advance(1000);
    for(unsigned i=0;i<preset;i++){web_enc(EN_PRESET,1);advance(100);}
    FILE *f=fopen(argv[2],"wb");assert(f);
    unsigned frames=44100*10;wav_hdr(f,frames);
    int kind=REGGAE_BANK[preset].kind,held[3]={0};
    unsigned step=0,next=0,off=0;double energy[8]={0};float peak=0;
    for(unsigned frame=0;frame<frames;frame+=128){
        if(frame>=off)for(unsigned j=0;j<3;j++)if(held[j]){web_midi(0x80,held[j],0);held[j]=0;}
        if(frame>=next && step<8){
            static const int melody[]={60,63,65,67,70,67,65,60};
            held[0]=kind==RG_DRUM?DRUM_LANE_NOTE[step]:melody[step]-(kind==RG_BASS?24:0);
            if(kind==RG_PIANO || kind==RG_BRASS || kind==RG_STRING || kind==RG_ORGAN){held[1]=held[0]+3;held[2]=held[0]+7;}
            for(unsigned j=0;j<3;j++)if(held[j])web_midi(0x90,held[j],96);
            off=frame+(kind==RG_REED || kind==RG_BASS || kind==RG_SIREN?22050:6615);
            next=++step*44100;
        }
        unsigned n=frames-frame<128?frames-frame:128;web_render(n);
        for(unsigned i=0;i<n;i++){
            float l=out_l[i],r=out_r[i];assert(isfinite(l)&&isfinite(r));
            if(fabsf(l)>peak)peak=fabsf(l);if(fabsf(r)>peak)peak=fabsf(r);
            if(frame<8*44100)energy[frame/44100]+=l*l+r*r;
            wav_put(f,(int32_t)(l*32768),(int32_t)(r*32768));
        }
    }
    fclose(f);assert(peak<0.99f);
    for(unsigned j=0;j<8;j++){assert(energy[j]>.01);printf("preset=%u strike=%u energy=%.4f peak=%.4f\n",preset,j,energy[j],peak);}
    advance(10000);
    for(unsigned v=0;v<NVOICE;v++)assert(!TSEL->v[v].active);
    return 0;
}
