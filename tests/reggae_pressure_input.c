/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void tap(unsigned k){web_buttons(1u<<k);advance(70);web_buttons(0);advance(400);}
static void samples(unsigned key,unsigned value){for(unsigned i=0;i<32;i++){fm1_ms+=3;pressure_sample(key,value,1);}}
int main(void){
 web_nor_erase();web_boot();advance(1000);assert(!pressure_mode);
 for(unsigned i=0;i<12&&cur_page()->graph!=GR_PRESSURE;i++)tap(B_FX);
 assert(cur_page()->graph==GR_PRESSURE&&!ui.lock);assert(kb_lock==1);
 web_enc(EN_K1,1);advance(100);assert(pressure_mode==1);
 web_keys(1);advance(100);assert(pressure_selected==0&&kb_note[0]==KB_SILENT);
 samples(0,800);edit_param(2,1);assert(pressure_keys[0].light==800);
 samples(0,500);edit_param(3,1);assert(pressure_calibrated(0));
 edit_param(0,1);assert(pressure_mode==1); /* live requires released keys */
 web_keys(0);advance(100);edit_param(0,1);assert(pressure_mode==2);
 assert(perf_key(0)==PF_ETHR&&perf_key(2)==PF_RTHR);
 uint32_t v=123;assert(!pressure_value(20,&v)&&v==123); /* unsupported */
 pressure_sample(0,0,0);assert(pressure_value(0,&v)&&v==0); /* missing calibrated = dry */
 samples(0,650);assert(pressure_value(0,&v)&&v==16384);
 fm1_ms+=41;assert(pressure_value(0,&v)&&v==0);
 pressure_sample(0,0,0);samples(0,300);assert(pressure_value(0,&v)&&v==32768);
 pressure_sample(0,0,0);samples(0,900);assert(pressure_value(0,&v)&&v==0);
 /* Open actual persistent punch page; held pressure updates without new edges. */
 tap(B_FX);for(unsigned i=0;i<12&&cur_page()->graph!=GR_PUNCH;i++)tap(B_FX);
 assert(cur_page()->graph==GR_PUNCH&&ui.lock==LAYER_FX);
 web_keys(1);advance(100);samples(0,650);keyboard_block();
 assert(perf_pressure_valid&PF_BIT(PF_ETHR));assert(perf_pressure[PF_ETHR]==16384);
 samples(0,500);keyboard_block();assert(perf_pressure[PF_ETHR]>32000);
 web_keys(0);advance(50);assert(!(perf_pressure_valid&PF_BIT(PF_ETHR)));
 ui.force=1;advance(150);picture("build/reggae/pressure-punch-screen.ppm");
 tap(B_FX);assert(cur_page()->graph==GR_PRESSURE);pressure_status="CALIBRATED";
 ui.force=1;advance(150);picture("build/reggae/pressure-screen.ppm");
 edit_param(0,-2);assert(!pressure_mode);keyboard_block();assert(!perf_pressure_valid);
 assert(perf_key(0)==PF_R8);
 puts("PASS: calibration, stale/saturated/unsupported sensing, continuous held-key FX, silent diagnostic keys, actual FX-page navigation, OFF fallback; synthetic samples only");
}
