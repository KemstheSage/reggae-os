/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
static double energy(double *left,double *right,unsigned blocks){
 *left=*right=0;
 for(unsigned b=0;b<blocks;b++){web_render(128);for(unsigned i=0;i<128;i++){assert(isfinite(out_l[i])&&isfinite(out_r[i]));*left+=out_l[i]*out_l[i];*right+=out_r[i]*out_r[i];}}
 return *left+*right;
}
int main(void){
 web_nor_erase();web_boot();advance(1000);tap(B_HOME);tap(B_HOME);
 assert(cur_page()->graph==GR_TRK&&reggae_seq_screen());double l,r;
 web_enc(EN_K2,-1000);advance(200);assert(TSEL->p[P_PAN]==-64);
 web_midi(0x90,60,100);advance(200);energy(&l,&r,100);assert(l>.01&&l>r*10);
 web_midi(0x80,60,0);advance(3000);
 web_enc(EN_K2,1000);advance(200);assert(TSEL->p[P_PAN]==63);
 web_midi(0x90,60,100);advance(200);energy(&l,&r,100);assert(r>.01&&r>l*10);
 web_midi(0x80,60,0);advance(3000);TSEL->p[P_PAN]=0;
 double tails[2];
 for(unsigned wet=0;wet<2;wet++){
  web_enc(EN_K3,wet?1000:-1000);advance(200);assert(TSEL->p[P_REV]==(wet?127:0));
  web_midi(0x90,60,100);advance(500);web_midi(0x80,60,0);advance(700);
  tails[wet]=energy(&l,&r,300);advance(10000);
 }
 assert(tails[0]<1e-7&&tails[1]>.001);
 printf("PASS: physical pan knob steers stereo audio; reverb knob creates tail (dry %.8f, wet %.8f)\n",tails[0],tails[1]);
}
