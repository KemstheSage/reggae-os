/* SPDX-License-Identifier: GPL-3.0-only
 * REGGAE-OS experimental contact-pressure model. Calibration is RAM-only.
 * ISR samples; UI captures under IRQ exclusion. No musical note velocity changes. */
#define PRESSURE_KEYS 20u
static volatile uint8_t pressure_mode; /* OFF / TEST / LIVE; always OFF at boot */
static uint8_t pressure_selected;
static const char *pressure_status = "OFF AT BOOT";
static volatile uint32_t pressure_faults;
static volatile struct {
    volatile uint32_t at, batch_at;
    volatile uint16_t raw, smooth, mean, spread;
    uint32_t sum;
    uint16_t lo, hi;
    uint8_t n, fresh;
    uint16_t light, firm;
    uint8_t captures;
} pressure_keys[PRESSURE_KEYS];
static int pressure_scan_enabled(void) { return pressure_mode != 0; }
static void pressure_scan_fault(void) { pressure_faults++; }
static void pressure_sample(uint32_t k, uint32_t raw, int valid)
{
    if(k >= PRESSURE_KEYS) return;
    __typeof__(pressure_keys[0]) *p=&pressure_keys[k];
    if(!valid) {p->fresh=0;p->n=0;p->batch_at=0;return;}
    raw=raw>1023u?1023u:raw;
    p->raw=raw;p->smooth=p->fresh?(3u*p->smooth+raw*16u)/4u:raw*16u;
    p->at=fm1_ms;p->fresh=1;
    if(!p->n){p->sum=0;p->lo=1023;p->hi=0;}
    p->sum+=raw;if(raw<p->lo)p->lo=raw;if(raw>p->hi)p->hi=raw;
    if(++p->n==32){p->mean=p->sum/32;p->spread=p->hi-p->lo;p->batch_at=fm1_ms;p->n=0;}
}
static int pressure_calibrated(uint32_t k)
{
    return k<PRESSURE_KEYS && pressure_keys[k].captures==3 &&
        (int)pressure_keys[k].light-(int)pressure_keys[k].firm>=32;
}
static int pressure_value(uint32_t k, uint32_t *value)
{
    if(pressure_mode!=2 || !pressure_calibrated(k))return 0;
    __typeof__(pressure_keys[0]) *p=&pressure_keys[k];
    *value=0;
    if(!p->fresh || fm1_ms-p->at>40u)return 1; /* failed calibrated sensor: dry */
    int32_t n=(int32_t)p->light-(int32_t)(p->smooth/16u);
    int32_t span=p->light-p->firm;
    *value=n<=0?0:n>=span?32768u:(uint32_t)(n*32768/span);
    return 1;
}
static void pressure_capture(uint32_t endpoint)
{
    uint32_t k=pressure_selected;
    if(k>=PRESSURE_KEYS){pressure_status="TOP 7: DIGITAL ONLY";return;}
    __typeof__(pressure_keys[0]) *p=&pressure_keys[k];
    if(!pressure_mode || !(fm1_in.notes&(1u<<k)) || !p->fresh ||
       !p->batch_at || fm1_ms-p->batch_at>150u || p->spread>60u){
        pressure_status="HOLD KEY STEADY";return;
    }
    if(endpoint==0)p->light=p->mean;else p->firm=p->mean;
    p->captures|=1u<<endpoint;
    pressure_status=pressure_calibrated(k)?"CALIBRATED":p->captures==3?"NEED MORE RANGE":endpoint?"FIRM CAPTURED":"LIGHT CAPTURED";
}
static uint32_t perf_effect_at(uint32_t p)
{
    /* Pressure must never hide or replace the user's effects. */
    return p<PF_KEYS?perf_map[p]:PF_N;
}
