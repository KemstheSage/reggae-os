/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
/* Synthetic pressure drives the actual DSP, not a browser effect. Physical
 * pressure sensing is deliberately not claimed by this test. */
static double mono_difference(unsigned pressure,int valid){
 memset(&pf,0,sizeof pf);perf_held=perf_latched=perf_act=0;perf_pressure_valid=0;
 perf_latch_on=0;perf_kill=0;perf_press(PF_MONO,1);perf_pressure_set(PF_MONO,pressure,valid);
 int32_t l[128],r[128];double sum=0;
 for(unsigned b=0;b<30;b++){
  for(unsigned i=0;i<128;i++){l[i]=8000;r[i]=-4000;}
  assert(perf_begin(128));perf_block(l,r,128);
  if(b==29)for(unsigned i=0;i<128;i++){sum+=abs(l[i]-r[i]);assert(abs(l[i])<=8000&&abs(r[i])<=8000);}
 }
 perf_press(PF_MONO,0);assert(!(perf_pressure_valid&PF_BIT(PF_MONO)));
 return sum;
}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 double light=mono_difference(4096,1),firm=mono_difference(24576,1),full=mono_difference(32768,1),fallback=mono_difference(0,0);
 assert(light>firm&&firm>full&&full==fallback&&full==0);
 perf_pressure_set(PF_GATE,8192,1);assert(perf_pressure_depth(PF_GATE,32768)==8192);
 assert(perf_pressure_depth(PF_MONO,32768)==32768); /* other effect unchanged */
 perf_latch_on=1;assert(perf_pressure_depth(PF_GATE,32768)==32768);perf_latch_on=0;
 perf_act=PF_BIT(PF_R8)|PF_BIT(PF_FLG)|PF_BIT(PF_PHS);
 perf_pressure_set(PF_R8,100,1);perf_pressure_set(PF_FLG,4096,1);perf_pressure_set(PF_PHS,24576,1);
 assert(perf_mod_depth(PF_FLG)==4096&&perf_mod_depth(PF_PHS)==24576);
 perf_pressure_set(PF_GATE,99999,1);assert(perf_pressure_depth(PF_GATE,32768)==32768);
 perf_pressure_set(PF_M1,100,1);assert(perf_pressure_depth(PF_M1,32768)==32768);
 memset(&pf,0,sizeof pf);perf_held=0;perf_press(PF_ETHR,1);perf_pressure_set(PF_ETHR,8192,1);
 for(unsigned n=0;n<20;n++)perf_begin(128);
 assert(pf.ew1==8192);perf_pressure_set(PF_ETHR,24576,1);
 for(unsigned n=0;n<20;n++)perf_begin(128);
 assert(pf.ew1==24576);perf_press(PF_ETHR,0);
 for(unsigned n=0;n<20;n++)perf_begin(128);assert(!pf.ew1);
 printf("PASS: synthetic continuous pressure changes real mono mix (%g > %g > %g), echo send ramps; full/fallback identity, latch/mute isolation, release cleanup\n",light,firm,full);
}
