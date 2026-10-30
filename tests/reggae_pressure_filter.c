/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static double run(unsigned raw,unsigned blocks,unsigned bpm){
 memset(&pf,0,sizeof pf);pf.lc=PF_TOP;perf_held=perf_latched=perf_act=0;
 memset((void*)perf_k,0,sizeof perf_k);perf_latch_on=perf_kill=0;
 song.g[G_BPM]=bpm;song.playing=0;pressure_mode=2;
 pressure_keys[7].light=800;pressure_keys[7].firm=420;pressure_keys[7].captures=3;
 pressure_sample(7,0,0);memset(kb_fx,0,sizeof kb_fx);fm1_in.notes=1u<<7;
 assert(perf_key(7)==PF_LPF);perf_key_press(7,1);
 double real=0,imag=0;unsigned phase=0;
 for(unsigned b=0;b<blocks;b++){
  pressure_sample(7,raw,1);perf_pressure_refresh();assert(perf_begin(128));
  int32_t l[128],r[128];
  for(unsigned i=0;i<128;i++,phase++){l[i]=r[i]=(int32_t)(10000*sin(2*3.141592653589793*phase/8.0));}
  perf_block(l,r,128);
  if(b>=blocks-4)for(unsigned i=0;i<128;i++){real+=l[i]*cos(2*3.141592653589793*i/8.0);imag+=l[i]*sin(2*3.141592653589793*i/8.0);}
 }
 return real*real+imag*imag;
}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 double open=run(800,32,120),mid=run(610,32,120),firm=run(420,32,120);
 assert(open>mid*4&&mid>firm*4);assert(pf.lc==PF_LOW);
 double slow=run(420,32,60);assert(pf.lc==PF_LOW&&fabs(slow-firm)<1);
 /* Ease pressure without releasing: cutoff opens again. */
 for(unsigned i=0;i<64;i++){pressure_sample(7,800,1);perf_pressure_refresh();perf_begin(128);}
 assert(pf.lc>PF_TOP-256);
 perf_key_press(7,0);fm1_in.notes=0;perf_pressure_refresh();
 assert(!(perf_pressure_valid&PF_BIT(PF_LPF)));
 /* Digital fallback keeps its original timed movement. */
 pressure_mode=0;memset(&pf,0,sizeof pf);pf.lc=PF_TOP;perf_held=0;perf_key_press(7,1);
 perf_pressure_refresh();perf_begin(128);assert(pf.lc>PF_TOP-256);
 /* HPF uses the same direct pressure mapping. */
 perf_held=PF_BIT(PF_HPF);perf_pressure_set(PF_HPF,16384,1);
 for(unsigned i=0;i<40;i++)perf_begin(128);
 assert(pf.hc==(PF_TOP-PF_LOW)/2);
 printf("PASS: physical LPF key -> raw 800/610/420 -> actual 5.5kHz spectral energy %.0f > %.0f > %.0f; direct cutoff independent of tempo, easing without release, digital fallback, HPF\n",open,mid,firm);
}
