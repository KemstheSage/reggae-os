/* SPDX-License-Identifier: GPL-3.0-only
 * Original audition phrases played through the firmware MIDI/audio paths.
 * Offline evidence only, not a separate browser instrument or sequencer feature. */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(int argc,char **argv) {
    assert(argc==3); unsigned preset=atoi(argv[1]); assert(preset<4);
    web_nor_erase(); web_boot(); advance(1000);
    for(unsigned i=0;i<preset;i++){web_enc(EN_PRESET,1);advance(100);}
    unsigned total=44100*8; FILE *f=fopen(argv[2],"wb");assert(f);wav_hdr(f,total);
    unsigned step=0,next=0,off=0; int held[3]={0};
    for(unsigned frame=0;frame<total;frame+=128) {
        if(frame>=off) for(unsigned n=0;n<3;n++) if(held[n]){web_midi(0x80,held[n],0);held[n]=0;}
        if(frame>=next && step<32) {
            unsigned pos=step%16;
            if(preset<2 && pos%4!=0) {
                int root=step<16?60:65;
                held[0]=root+3;held[1]=root+7;held[2]=pos%4==2?root+12:0;
                for(unsigned n=0;n<3;n++)if(held[n])web_midi(0x90,held[n],pos%4==2?100:78);
                off=frame+(pos%4==2?3528:2205);
            } else if(preset>=2) {
                static const int notes[16]={36,0,0,36,0,0,31,0,34,0,0,31,0,36,0,0};
                if(notes[pos]){held[0]=notes[pos]+(step>=16?5:0);web_midi(0x90,held[0],108);off=frame+(pos==0?13230:6615);}
            }
            step++; next=(step*44100u*3u)/16u; /* 80 BPM sixteenths */
        }
        unsigned n=total-frame<128?total-frame:128;
        web_render(n);
        for(unsigned i=0;i<n;i++)wav_put(f,(int32_t)(out_l[i]*32768),(int32_t)(out_r[i]*32768));
    }
    fclose(f);return 0;
}
