/* SPDX-License-Identifier: GPL-3.0-only
 * Register-model test; validates scheduling/restoration, not electrical behaviour. */
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t regs[8],con,res,wla;
#define FM1_PR(p,r) regs[(r)/4]
#define FM1_ADC_CON con
#define FM1_ADC_RES res
#define FM1_WLA_CON0 wla
enum {FM1_PA,FM1_OUT=0,FM1_DIR=8,FM1_HD0=24,FM1_HD=28,FM1_ADC_BATT=3,FM1_ADC_MASTER=4};
static uint8_t fm1_adc_main_busy,fm1_adc_scan_owned;
static int32_t fm1_adc_master_cache=-1,fm1_adc_batt_cache=-1;
static struct {uint32_t frames,notes;} fm1_in;
static const int8_t FM1_KEYMAP[6][11]={
 {0},{0},{0},{23,24,25,26,27,28,29,30,31,32,33},{0,1,15,14,17,16,19,18,20,21,22},{0}};
static unsigned enabled,faults,seen,last_valid,last_value;
static int pressure_scan_enabled(void){return enabled;}
static void pressure_scan_fault(void){faults++;}
static void pressure_sample(uint32_t k,uint32_t v,int valid){assert(k<20);if(valid)seen|=1u<<k;last_valid=valid;last_value=v;}
#include "../firmware/hal/fm1_pressure.h"
int main(void){
 uint32_t initial[8];for(unsigned i=0;i<8;i++)regs[i]=0xA5A5FFFFu;memcpy(initial,regs,sizeof regs);
 fm1_pressure_before();fm1_pressure_after(3);assert(!fm1_adc_scan_owned&&!con);
 enabled=1;fm1_adc_main_busy=1;fm1_pressure_before();assert(!fm1_adc_scan_owned);
 fm1_adc_main_busy=0;fm1_in.notes=(1u<<27)-1;
 for(unsigned row=0;row<2;row++)for(unsigned col=0;col<11;col++){
  fm1_in.frames=row;fm1_pressure_before();fm1_pressure_after(col);
  assert(fm1_pressure.pending);res=600;con|=0x80;fm1_pressure_before();
  assert(!memcmp(regs,initial,sizeof regs));
 }
 assert(seen==((1u<<20)-1));assert(fm1_adc_master_cache==600&&fm1_adc_batt_cache==600);
 fm1_pressure_after(3);con&=~0x80;fm1_pressure_before();assert(faults==1&&!last_valid);
 assert(!memcmp(regs,initial,sizeof regs));
 fm1_pressure_after(3);assert(fm1_pressure.pending);enabled=0;res=400;con|=0x80;fm1_pressure_before();
 assert(!fm1_adc_scan_owned&&!fm1_pressure.pending&&!con);assert(!memcmp(regs,initial,sizeof regs));
 puts("PASS: 20-key scan coverage, master/battery scheduling, ADC exclusion, timeout, OFF drains pending conversion and restores GPIO");
}
