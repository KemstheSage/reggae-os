/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(void) {
 web_nor_erase();web_boot();advance(1000);
 track_t *t=&trk[0];t->p[P_VOICE]=V_POLY;t->p[P_CHRD]=CH_MAJ;t->p[P_VOIC]=VC_AUTO;
 assert(TP[P_VOIC].max==VC_AUTO);
 uint8_t nn[4], preview[4];int32_t root;uint16_t mask;
 assert(chord_build(t,60,nn)==3 && !memcmp(nn,(uint8_t[]){60,64,67},3));
 /* Preview drawing must never alter performance history. */
 chord_make(t,90,preview,&root,&mask);
 assert(chord_build(t,65,nn)==3 && !memcmp(nn,(uint8_t[]){60,65,69},3));
 assert(chord_build(t,67,nn)==3 && !memcmp(nn,(uint8_t[]){62,67,71},3));
 assert(chord_build(t,60,nn)==3 && !memcmp(nn,(uint8_t[]){64,67,72},3));
 t->p[P_VOIC]=VC_CLOSE;chord_build(t,60,nn);t->p[P_VOIC]=VC_AUTO;
 assert(chord_build(t,65,nn)==3 && nn[0]==65);
 assert(chord_build(t,77,nn)==3 && nn[0]==77); /* deliberate octave jump */
 trk[1].p[P_VOICE]=V_POLY;trk[1].p[P_CHRD]=CH_MAJ;trk[1].p[P_VOIC]=VC_AUTO;
 chord_build(&trk[1],48,nn);assert(nn[0]==48);assert(chord_lead[0].note[0]==77);
 for(unsigned c=1;c<CH_COUNT;c++)for(unsigned r=0;r<128;r++) {
  t->p[P_CHRD]=c;unsigned n=chord_build(t,r,nn);
  uint8_t plain[4];unsigned np=chord_make(t,r,plain,&root,&mask);assert(n==np);
  unsigned a=0,b=0;for(unsigned i=0;i<n;i++){assert(nn[i]<128);if(i)assert(nn[i]>nn[i-1]);a|=1u<<(nn[i]%12);b|=1u<<(plain[i]%12);}assert(a==b);
 }
 midi_forget_track(0);assert(!chord_lead[0].n);
 t->p[P_CHRD]=CH_MAJ;t->p[P_VOIC]=VC_AUTO;
 web_midi(0x90,60,100);advance(100);web_midi(0x90,65,100);advance(100);
 double energy=0;for(unsigned j=0;j<128;j++)energy+=out_l[j]*out_l[j]+out_r[j]*out_r[j];assert(energy>1e-6);
 t->p[P_CHRD]=CH_MIN9;t->p[P_VOIC]=VC_CLOSE;
 web_midi(0x80,60,0);web_midi(0x80,65,0);advance(1800);
 for(unsigned i=0;i<NVOICE;i++)assert(!t->v[i].gate);
 t->p[P_VOIC]=VC_AUTO;assert(project_save(0)==0);t->p[P_VOIC]=VC_CLOSE;project_load(0);assert(t->p[P_VOIC]==VC_AUTO);
 midi_forget_track(0);t->p[P_CHRD]=CH_MAJ;
 web_keys(1u<<7);advance(150);assert(chord_lead[0].n==3);web_keys(0);advance(150);
 web_keys(1u<<12);advance(150);assert(chord_lead[0].n==3);web_keys(0);advance(1800);
 for(unsigned i=0;i<NVOICE;i++)assert(!t->v[i].gate);
 for(unsigned tap=0;tap<2;tap++){web_buttons(1u<<B_SCL);advance(70);web_buttons(0);advance(400);}
 assert(cur_page()->graph==GR_CHORD);ui.msg_t=0;ui.force=1;advance(150);picture("build/reggae/auto-voicing.ppm");
 t->p[P_VOICE]=V_MONO;assert(chord_build(t,60,nn)==1 && nn[0]==60 && !chord_lead[0].n);
 puts("PASS: AUTO progression, preview isolation, octave reset, track isolation, all shapes/range/pitch classes, overlapping MIDI release, audible audio, project save");
}
