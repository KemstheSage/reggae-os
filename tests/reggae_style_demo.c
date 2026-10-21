/* SPDX-License-Identifier: GPL-3.0-only
 * Original performance studies: rhythm is in MIDI events, never baked into DSP. */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(int argc,char **argv) {
    assert(argc==3);unsigned p=atoi(argv[1]);assert(p==4||p==7||p==8||p==9||p==12);
    web_nor_erase();web_boot();advance(1000);
    for(unsigned i=0;i<p;i++){web_enc(EN_PRESET,1);advance(100);}
    FILE *f=fopen(argv[2],"wb");assert(f);const unsigned frames=44100*16,step=16538;
    wav_hdr(f,frames);unsigned tick=0,off=0;int held[3]={0};float peak=0;double energy=0;
    for(unsigned frame=0;frame<frames;frame+=128) {
        if(frame>=off)for(unsigned j=0;j<3;j++)if(held[j]){web_midi(0x80,held[j],0);held[j]=0;}
        if(tick<32 && frame>=tick*step) {
            int fire=0;
            if(p==12) {
                /* 80 BPM one-drop: kick + cross-stick on beat three; light hats. */
                web_midi(0x90,42,tick%2?76:48);
                if(tick%8==4){web_midi(0x90,36,100);web_midi(0x90,37,105);}
                if(tick==15||tick==31)web_midi(0x90,45,80);
            } else if(p==8 && tick%2) {
                int root=(tick/8)%2?65:60;
                held[0]=root+7;held[1]=root+12;held[2]=root+15;
                fire=1;off=frame+4410; /* 100 ms compact upper-voiced skanks */
            } else if(p==7 && (tick%8==3||tick%8==6)) {
                held[0]=60+(tick/8)%2*5;held[1]=held[0]+3;held[2]=held[0]+7;
                fire=1;off=frame+(tick%8==3?11025:6615);
            } else if(p==4 && tick%2==0) {
                static const int notes[8]={67,70,72,74,72,70,67,65};
                fire=1;held[0]=notes[(tick/2)%8];off=frame+(tick%8==6?27562:16538);
            } else if(p==9 && tick%8==0) {
                fire=1;held[0]=72;off=frame+66150;
                if(tick==8)web_enc(EN_K2,12);
                if(tick==16)web_enc(EN_K1,12);
                if(tick==24)web_enc(EN_K2,-24);
            }
            if(fire)for(unsigned j=0;j<3;j++)if(held[j])web_midi(0x90,held[j],90);
            tick++;
        }
        unsigned n=frames-frame<128?frames-frame:128;web_render(n);
        for(unsigned i=0;i<n;i++) {
            assert(isfinite(out_l[i])&&isfinite(out_r[i]));
            float a=fmaxf(fabsf(out_l[i]),fabsf(out_r[i]));if(a>peak)peak=a;
            energy+=out_l[i]*out_l[i]+out_r[i]*out_r[i];
            wav_put(f,(int32_t)(out_l[i]*32768),(int32_t)(out_r[i]*32768));
        }
    }
    fclose(f);assert(energy>0.1 && peak<0.99);advance(10000);
    for(unsigned i=0;i<NVOICE;i++)assert(!TSEL->v[i].active);
    printf("style=%u peak=%.5f energy=%.5f\n",p,peak,energy);
}
