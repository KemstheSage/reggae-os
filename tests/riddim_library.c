/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#define RIDDIM_SPLASH_MS 3000u
#include "reggae_test.c"
int main(int argc,char **argv){
 assert(argc==2);unsigned preset=atoi(argv[1]);assert(preset<NELEM(REGGAE_BANK));
 web_nor_erase();web_boot();
 uint16_t splash[240*240];memcpy(splash,web_fb,sizeof splash);advance(1500);
 assert(!memcmp(splash,web_fb,sizeof splash));picture("build/reggae/riddim-startup.ppm");advance(2000);
 assert(memcmp(splash,web_fb,sizeof splash));
 for(unsigned i=0;i<preset;i++){web_enc(EN_PRESET,1);advance(100);}
 assert(reggae_index(TSEL)==(int)preset);
 char path[128];snprintf(path,sizeof path,"build/reggae/riddim-%u.wav",preset);
 FILE *f=fopen(path,"wb");assert(f);unsigned frames=44100*7;wav_hdr(f,frames);
 web_midi(0x90,60,96);double energy=0,late=0,tail=0;float peak=0;
 for(unsigned frame=0;frame<frames;frame+=128){
  if(frame>=44100*4 && frame<44100*4+128)web_midi(0x80,60,0);
  unsigned n=frames-frame<128?frames-frame:128;web_render(n);
  for(unsigned i=0;i<n;i++){
   float l=out_l[i],r=out_r[i];assert(isfinite(l)&&isfinite(r));
   if(fabsf(l)>peak)peak=fabsf(l);if(fabsf(r)>peak)peak=fabsf(r);
   energy+=l*l+r*r;if(frame>44100*3&&frame<44100*4)late+=l*l+r*r;
   if(frame>44100*6)tail+=l*l+r*r;
   wav_put(f,l*32768,r*32768);
  }
 }
 fclose(f);assert(energy>.01&&peak<.99);
 if(preset==7||preset==20||preset==21){
  unsigned set=TSEL->p[P_E0];const smp_set_t *ss=smp_set(set);
  for(unsigned i=0;i<ss->nz;i++)assert(!SMP_ZONES[ss->z0+i].looped);
  assert(late<.001); /* held sampled note ends instead of repeating */
 }
 if(preset==19)assert(late>.01); /* synthesized brass sustains */
 advance(10000);for(unsigned i=0;i<NVOICE;i++)assert(!TSEL->v[i].active);
 ui.force=1;advance(100);snprintf(path,sizeof path,"build/reggae/riddim-%u.ppm",preset);picture(path);
 printf("PASS preset %u: energy %.5f peak %.5f held-tail %.5f release-tail %.5f; 3s splash and note release\n",preset,energy,peak,late,tail);
}
