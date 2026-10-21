/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(void) {
    web_nor_erase();web_boot();advance(1000);
    for(unsigned i=0;i<13;i++){web_enc(EN_PRESET,1);advance(100);}
    assert(reggae_index(TSEL)==13);
    TSEL->p[P_DLY]=0;
    for(unsigned hit=0;hit<4;hit++) {
        web_midi(0x90,60,100);advance(40);
        int active=0;for(unsigned v=0;v<NVOICE;v++)active+=TSEL->v[v].active;
        assert(active);
        /* A held key must decay away, rather than remain a repeating wail. */
        advance(5000);
        double e=0;web_render(128);
        for(unsigned i=0;i<128;i++)e+=out_l[i]*out_l[i]+out_r[i]*out_r[i];
        assert(e<0.00001);
        web_midi(0x80,60,0);advance(1000);
        for(unsigned v=0;v<NVOICE;v++)assert(!TSEL->v[v].active);
    }
    puts("PASS: Jungle Signal retriggers four times and decays to silence with key held");
}
