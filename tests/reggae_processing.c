/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
static double tone(unsigned unit,double hz,int amplitude,int mode){
 double e=0;int32_t l[128],r[128];
 for(unsigned block=0;block<400;block++){
  for(unsigned i=0;i<128;i++){l[i]=amplitude*sin((block*128+i)*6.283185307179586*hz/FS);r[i]=l[i]/2;}
  rp_process(unit,l,r,128,1);
  for(unsigned i=0;i<128;i++){assert(abs(l[i])<1048576);assert(abs(r[i])<1048576);
   if(block>250)e+=(double)l[i]*l[i];if(mode)assert(abs(l[i]-r[i]*2)<32);}
 }
 return sqrt(e/(149*128));
}
int main(void){
 web_nor_erase();web_boot();advance(1000);
 int32_t a[128],b[128];for(unsigned i=0;i<128;i++)a[i]=b[i]=i*313-17000;
 rp_process(0,a,0,128,0);assert(!memcmp(a,b,sizeof a));
 for(unsigned u=0;u<5;u++){
  memset(rp_config,0,sizeof rp_config);rp_reset();double dry=tone(u,RP_HZ[16],2000,0);
  rp_config[u].v[RP_FREQ]=16;rp_config[u].v[RP_GAIN]=12;rp_config[u].v[RP_Q]=2;
  double boost=tone(u,RP_HZ[16],2000,0);assert(boost/dry>3.8&&boost/dry<4.2);
  rp_config[u].v[RP_GAIN]=-12;double cut=tone(u,RP_HZ[16],2000,0);assert(cut/dry>.23&&cut/dry<.28);
  printf("unit %u EQ boost %.2f dB, cut %.2f dB\n",u,20*log10(boost/dry),20*log10(cut/dry));
 }
 memset(rp_config,0,sizeof rp_config);rp_reset();
 rp_config[0].v[RP_THRESH]=24;rp_config[0].v[RP_RATIO]=3;rp_config[0].v[RP_ATTACK]=3;rp_config[0].v[RP_RELEASE]=150;
 double low=tone(0,1000,5000,1),high=tone(0,1000,20000,1);
 printf("4:1 compressor: 12dB input rise -> %.2fdB output rise\n",20*log10(high/low));
 assert(high/low>1.3&&high/low<1.6);assert(rp_state[0].reduction>1000);
 /* Extremes, both channels, filter silence decay. */
 for(unsigned hz=0;hz<32;hz+=31)for(int gain=-12;gain<=12;gain+=24)for(unsigned q=0;q<=6;q+=6){
  rp_config[0].v[RP_FREQ]=hz;rp_config[0].v[RP_GAIN]=gain;rp_config[0].v[RP_Q]=q;
  tone(0,RP_HZ[hz],12000,0);
  memset(a,0,sizeof a);for(unsigned n=0;n<1200;n++){memset(a,0,sizeof a);rp_process(0,a,0,128,0);}
  for(unsigned i=0;i<128;i++)assert(abs(a[i])<8);
 }
 /* Legacy remains legacy; new settings round trip in the same flash slots. */
 memset(rp_config,0,sizeof rp_config);assert(project_save(0)==0);assert(*(uint32_t*)proj_wire.raw==PROJ_MAGIC);
 rp_config[0].v[RP_GAIN]=5;rp_config[4].v[RP_RATIO]=3;rp_config[4].v[RP_THRESH]=18;
 assert(project_save(1)==0);assert(*(uint32_t*)proj_wire.raw==PROJ_MAGIC_EXT);
 project_store_t saved=proj_wire;project_t decoded;
 assert(proj_import(&decoded,&saved,sizeof saved));assert(decoded.processing[4].v[RP_THRESH]==18);
 project_load(0);assert(!rp_config[0].v[RP_GAIN]&&!rp_config[4].v[RP_RATIO]);
 project_load(1);assert(rp_config[0].v[RP_GAIN]==5&&rp_config[4].v[RP_RATIO]==3);
 saved.raw[160]^=1;assert(!proj_import(&decoded,&saved,sizeof saved));
 /* Bounded codec rejects truncation/overflow instead of writing off the end. */
 uint8_t noise[3840],packed[3840],round[3840];for(unsigned i=0;i<sizeof noise;i++)noise[i]=(i*43+i/127)&255;
 assert(!proj_rle(packed,100,noise,sizeof noise));
 memset(noise,0,sizeof noise);unsigned n=proj_rle(packed,sizeof packed,noise,sizeof noise);assert(n);
 assert(proj_unrle(round,sizeof round,packed,n)&&!memcmp(noise,round,sizeof noise));assert(!proj_unrle(round,sizeof round,packed,n-1));
 tap(B_HOME);for(unsigned i=0;i<20&&cur_page()->scope!=SC_PROCESS;i++)tap(B_FX);
 assert(cur_page()->scope==SC_PROCESS);web_enc(EN_K1,4);advance(100);assert(rp_target==4);
 web_enc(EN_K3,3);advance(100);assert(rp_config[4].v[RP_GAIN]==3);
 ui.msg_t=0;ui.force=1;advance(100);picture("build/reggae/processing-eq.ppm");tap(B_FX);ui.force=1;advance(100);picture("build/reggae/processing-comp.ppm");
 puts("PASS processing: bypass, five EQ paths, linked compressor ratio, bounded extremes, old/new project recall, corrupt data rejected, physical panel targets");
}
