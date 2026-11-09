/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static double level(unsigned ms) {
 double e=0;for(unsigned b=0;b<ms*441/10;b+=128){web_render(128);for(unsigned j=0;j<128;j++){assert(isfinite(out_l[j])&&fabs(out_l[j])<.99);e+=out_l[j]*out_l[j];}}return e;
}
int main(int argc,char **argv) {
 assert(argc==3||argc==4);int kit=atoi(argv[1]),legacy=kit<0;web_nor_erase();
 FILE*pack=fopen("build/reggae/drums/RDMDRM2.slot","rb");assert(pack);assert(fread(nor+SMP_USER_BASE,1,SMP_USER_SIZE,pack)>512);fclose(pack);web_boot();advance(1000);
 preset_go(riddim_position(kit>0?27+kit:12));advance(100);assert(TSEL->p[P_E0]>=DK_ROOTS);
 if(legacy){TSEL->p[P_E0]=DK_DEEP;TSEL->p[P_E2]=59;}
 else if(kit==0) {
  drum_lane_t *lanes=drum_kit_of(TSEL);
  web_midi(0x90,46,100);assert(level(30)>.001);assert(lanes[DV_HATO].v.live);
  web_midi(0x90,42,100);level(10);assert(!lanes[DV_HATO].v.live);
  web_midi(0x90,46,100);assert(level(30)>.001);assert(lanes[DV_HATO].v.live);
  level(2000);assert(!lanes[DV_HATO].v.live);
  double energies[2];for(int v=0;v<2;v++){web_midi(0x90,42,v?110:30);energies[v]=level(400);}
  assert(energies[1]>energies[0]*2);
  puts("PASS acoustic open-hat choke, retrigger after choke, natural end and velocity response");
 }
 advance(1000);FILE*f=fopen(argv[2],"wb");assert(f);unsigned frames=44100*8;wav_hdr(f,frames);
 unsigned step=0,next=0;double energy=0;float peak=0;
 for(unsigned at=0;at<frames;at+=128){
  if(at>=next&&step<32){
   if(step%4==0)web_midi(0x90,36,100);
   if(step%8==4)web_midi(0x90,37,100);
   if(step==12||step==28)web_midi(0x90,38,95);
   web_midi(0x90,step%8==7?46:42,step%2?60:90);
   next=++step*8269; /* sixteenths at approximately 80 BPM */
  }
  unsigned n=frames-at<128?frames-at:128;web_render(n);
  for(unsigned i=0;i<n;i++){assert(isfinite(out_l[i]));energy+=out_l[i]*out_l[i];if(fabsf(out_l[i])>peak)peak=fabsf(out_l[i]);wav_put(f,out_l[i]*32768,out_r[i]*32768);}
 }
 fclose(f);assert(energy>.01&&peak<.99);advance(5000);
 for(unsigned i=0;i<NVOICE;i++)assert(!TSEL->v[i].active);
 if(argc==4) {
  FILE *tour=fopen(argv[3],"wb");assert(tour);wav_hdr(tour,44100*16);
  for(unsigned lane=0;lane<8;lane++) {
   web_midi(0x90,DRUM_LANE_NOTE[lane],100);
   for(unsigned at=0;at<88200;at+=128) {
    unsigned n=88200-at<128?88200-at:128;web_render(n);
    for(unsigned i=0;i<n;i++)wav_put(tour,out_l[i]*32768,out_r[i]*32768);
   }
  }
  fclose(tour);
  char path[160];snprintf(path,sizeof path,"build/reggae/drums/kit-%d.ppm",kit);picture(path);
 }
 printf("PASS %s groove: energy %.4f peak %.4f\n",legacy?"legacy":"hybrid",energy,peak);
}
