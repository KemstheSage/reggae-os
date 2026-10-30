/* SPDX-License-Identifier: GPL-3.0-only
 * Experimental FM-1 contact sensing. PA7/PA8 only, weakest drive, opt-in.
 * ADC/row-drive technique adapted from Charles Vestal's fm1-velocity,
 * commit 8f62ce0d968c91ce8bfdafbb908f81d15351650a (GPL-3.0-only).
 * No upper-row LED probe. Digital scanning remains authoritative. */
static int pressure_scan_enabled(void);
static void pressure_sample(uint32_t key,uint32_t raw,int valid);
static void pressure_scan_fault(void);
static struct { uint32_t mask,out,dir,hd0,hd; uint8_t pending,kind,key; } fm1_pressure;
static void fm1_pressure_before(void)
{
    if(fm1_pressure.pending){
        int done=(FM1_ADC_CON&0x80u)!=0;
        uint32_t value=FM1_ADC_RES&1023u,m=fm1_pressure.mask;
        FM1_ADC_CON=0x40u;FM1_ADC_CON=0;
        if(m){
            /* Release before restoring output latch, then restore exact row state. */
            FM1_PR(FM1_PA,FM1_DIR)|=m;
            FM1_PR(FM1_PA,FM1_OUT)=(FM1_PR(FM1_PA,FM1_OUT)&~m)|fm1_pressure.out;
            FM1_PR(FM1_PA,FM1_HD0)=(FM1_PR(FM1_PA,FM1_HD0)&~m)|fm1_pressure.hd0;
            FM1_PR(FM1_PA,FM1_HD)=(FM1_PR(FM1_PA,FM1_HD)&~m)|fm1_pressure.hd;
            FM1_PR(FM1_PA,FM1_DIR)=(FM1_PR(FM1_PA,FM1_DIR)&~m)|fm1_pressure.dir;
        }
        if(!done)pressure_scan_fault();
        if(!fm1_pressure.kind)pressure_sample(fm1_pressure.key,value,done);
        else if(done && fm1_pressure.kind==1)fm1_adc_master_cache=value;
        else if(done)fm1_adc_batt_cache=value;
        fm1_pressure.pending=0;
    }
    if(!pressure_scan_enabled())fm1_adc_scan_owned=0;
    else if(!fm1_adc_main_busy)fm1_adc_scan_owned=1;
}
static void fm1_pressure_after(uint32_t col)
{
    if(!fm1_adc_scan_owned || !pressure_scan_enabled())return;
    uint32_t row=(fm1_in.frames&1u)?3u:4u, ch=row==3u?0u:1u;
    fm1_pressure.mask=0;fm1_pressure.kind=0;
    if(row==4u && col<2u){fm1_pressure.kind=col+1;ch=col?FM1_ADC_BATT:FM1_ADC_MASTER;}
    else {
        int key=FM1_KEYMAP[row][col]-14;
        if(key<0 || key>=20)return;
        fm1_pressure.key=key;
        if(!(fm1_in.notes&(1u<<key))){pressure_sample(key,0,0);return;}
        uint32_t m=1u<<(row==3u?7u:8u);
        fm1_pressure.mask=m;
        fm1_pressure.out=FM1_PR(FM1_PA,FM1_OUT)&m;
        fm1_pressure.dir=FM1_PR(FM1_PA,FM1_DIR)&m;
        fm1_pressure.hd0=FM1_PR(FM1_PA,FM1_HD0)&m;
        fm1_pressure.hd=FM1_PR(FM1_PA,FM1_HD)&m;
        FM1_PR(FM1_PA,FM1_HD0)&=~m;FM1_PR(FM1_PA,FM1_HD)&=~m;
        FM1_PR(FM1_PA,FM1_OUT)|=m;FM1_PR(FM1_PA,FM1_DIR)&=~m;
    }
    FM1_WLA_CON0&=~(1u<<14);
    FM1_ADC_CON=0;
    FM1_ADC_CON=(0x200Fu&0xF00Fu)|0x40u|(ch<<8);
    FM1_ADC_CON|=0x10u;FM1_ADC_CON|=0x40u;
    fm1_pressure.pending=1;
}
