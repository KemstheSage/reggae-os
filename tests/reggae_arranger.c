/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned key){web_buttons(1u<<key);advance(60);web_buttons(0);advance(100);}
int main(void) {
    web_nor_erase();web_boot();advance(1000);
    /* Two saved four-track source patterns; use the existing NOR project writer. */
    for(unsigned source=0;source<2;source++) {
        for(unsigned k=0;k<NTRK;k++) {
            memset(trk[k].step,0,sizeof trk[k].step);trk[k].p[P_SLEN]=4;
            trk[k].step[0].n=1;trk[k].step[0].note[0]=48+source*7+k*4;
            trk[k].step[0].vel=100;
        }
        assert(project_save(source)==0);
    }
    tap(9);assert(cur_page()->graph==GR_ROLL);assert(reggae_seq_screen());
    picture("build/reggae/sequence-notes.ppm");
    web_enc(EN_K1,1);advance(150);assert(ui.cursor==1);
    tap(5);assert(cur_page()->graph==GR_SONG);assert(reggae_seq_screen());
    chain_defaults(&chain_config);ui.song_row=ui.song_trk=0;ui.force=1;advance(150);
    picture("build/reggae/arrange-empty.ppm");
    web_enc(EN_K3,1);advance(150);assert(chain_config.count==1);
    web_enc(EN_K4,1);advance(150);assert(chain_config.row[0].repeat==2);
    web_enc(EN_K1,1);advance(150);web_enc(EN_K3,1);advance(150);
    assert(chain_config.count==2);
    for(unsigned k=0;k<NTRK;k++)chain_config.row[1].slot[k]=1;
    chain_config.row[0].slot[3]=CHAIN_SILENT;
    assert(project_save(3)==0);
    ui.force=1;advance(150);picture("build/reggae/arrange-edit.ppm");
    tap(10);assert(chain.running&&song.playing);
    unsigned seen=0;double energy=0;
    for(unsigned n=0;n<1000;n++) {
        web_render(128);seen|=1u<<chain.row;
        for(unsigned j=0;j<128;j++){assert(isfinite(out_l[j]));energy+=out_l[j]*out_l[j];}
        if(n==200)picture("build/reggae/arrange-playing.ppm");
    }
    advance(8000);assert(!chain.running&&!song.playing);assert(energy>0.1);assert((seen&3)==3);
    /* Saved arrangement, repeats and silent lane survive project recall. */
    chain_defaults(&chain_config);project_load(3);
    assert(chain_config.count==2&&chain_config.row[0].repeat==2&&chain_config.row[0].slot[3]==CHAIN_SILENT);
    assert(chain_config.row[1].slot[0]==1);
    /* Last section scrolls safely; STEP switches to actual eight-lane drum editing. */
    chain_config.count=16;ui.song_row=15;ui.force=1;advance(150);
    picture("build/reggae/arrange-last.ppm");
    ui.home=1;select_engine(10);apply_preset(4);TSEL->p[P_SLEN]=16;
    memset(TSEL->step,0,sizeof TSEL->step);tap(9);ui.cursor=0;ui.lane=0;
    web_enc(EN_K3,1);advance(150);assert(step_lanes(&TSEL->step[0])&1);
    picture("build/reggae/sequence-drums.ppm");
    puts("PASS: physical SEQ/GLO navigation, step edit, add sections, repeats, audible song, stop and NOR recall");
}
