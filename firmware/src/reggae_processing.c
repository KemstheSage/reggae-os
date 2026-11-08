/* SPDX-License-Identifier: GPL-3.0-only */
/* One fully parametric bell and stereo-linked feed-forward peak compressor per
 * track and master. Fixed-point sample loop; bounded DF1/Q28 biquad with int64
 * accumulator. RBJ peaking coefficients. Config changes fade EQ out before
 * replacing coefficients/states, then fade in. No heap or browser dependencies. */
static const uint16_t RP_HZ[] = {40,49,59,71,87,105,128,155,188,228,276,335,407,493,599,726,881,1069,1297,1574,1909,2316,2810,3409,4136,5018,6087,7385,8960,10870,13188,16000};
static const int32_t RP_A[] = {525533,556673,589658,624597,661607,700809,742335,786321,832914,882267,934544,989920,1048576,1110708,1176522,1246235,1320079,1398299,1481153,1568917,1661881,1760354,1864661,1975149,2092184};
static const uint16_t RP_Q100[] = {50,70,100,140,200,300,400};
static const uint8_t RP_RATIO_VALUE[] = {1,2,3,4,8,20};
static const uint16_t RP_LOG[] = {0,46,92,137,182,226,271,315,358,402,445,487,530,572,613,655,696,737,778,818,858,898,937,977,1016,1054,1093,1131,1169,1207,1244,1282,1319,1355,1392,1428,1465,1500,1536,1572,1607,1642,1677,1712,1746,1780,1814,1848,1882,1915,1949,1982,2015,2047,2080,2112,2145,2177,2208,2240,2272,2303,2334,2365,2396,2427,2457,2488,2518,2548,2578,2608,2637,2667,2696,2725,2754,2783,2812,2841,2869,2897,2926,2954,2982,3009,3037,3065,3092,3119,3146,3174,3200,3227,3254,3280,3307,3333,3359,3386,3412,3437,3463,3489,3514,3540,3565,3590,3615,3640,3665,3690,3715,3739,3764,3788,3812,3836,3861,3885,3908,3932,3956,3979,4003,4026,4050,4073,4096};
static const uint16_t RP_EXP[] = {32768,32591,32415,32240,32066,31893,31720,31549,31379,31209,31041,30873,30706,30541,30376,30212,30048,29886,29725,29564,29405,29246,29088,28931,28774,28619,28464,28311,28158,28006,27855,27704,27554,27406,27258,27110,26964,26818,26674,26530,26386,26244,26102,25961,25821,25681,25543,25405,25268,25131,24995,24860,24726,24593,24460,24328,24196,24066,23936,23806,23678,23550,23423,23296,23170,23045,22921,22797,22674,22552,22430,22309,22188,22068,21949,21831,21713,21595,21479,21363,21247,21133,21019,20905,20792,20680,20568,20457,20347,20237,20127,20019,19911,19803,19696,19590,19484,19379,19274,19170,19066,18963,18861,18759,18658,18557,18457,18357,18258,18160,18061,17964,17867,17770,17674,17579,17484,17390,17296,17202,17109,17017,16925,16834,16743,16652,16562,16473,16384};
static const int32_t RP_MAKE[] = {32768,36766,41252,46286,51934,58271,65381,73358,82309,92353,103622,116265,130452};

typedef struct {int32_t x1,x2,y1,y2;} rp_hist_t;
typedef struct {
 int32_t b[5];rp_hist_t h[2];reggae_proc_config_t applied;
 int32_t wet,env,gain,goal;uint32_t phase;uint16_t reduction;
} rp_state_t;
static rp_state_t rp_state[NTRK+1] __attribute__((section(".pool")));
static void rp_reset(void){memset(rp_state,0,sizeof rp_state);for(unsigned i=0;i<NTRK+1;i++)rp_state[i].gain=32768;}
static int32_t rp_log2(uint32_t x){
 if(!x)return 0;int n=0;uint32_t z=x;while(z>=256){z>>=1;n++;}
 while(z<128){z<<=1;n--;}
 return ((int32_t)n+7)*4096+RP_LOG[z-128];
}
static int32_t rp_expneg(uint32_t x){unsigned whole=x>>12,idx=(x&4095)>>5;return whole>15?0:RP_EXP[idx]>>whole;}
/* Freestanding target has no signed 64-bit divide runtime. Coefficient-only
 * bounded long division: quotient fits int32, never runs per sample. */
static int32_t rp_div(int64_t numerator,uint32_t denominator){
 int negative=numerator<0;uint64_t n=negative?-numerator:numerator;uint32_t q=0;
 for(int bit=30;bit>=0;bit--){uint64_t d=(uint64_t)denominator<<bit;if(n>=d){n-=d;q|=1u<<bit;}}
 return negative?-(int32_t)q:(int32_t)q;
}
static void rp_coeff(rp_state_t *s,const reggae_proc_config_t *c){
 uint32_t ph=(uint64_t)RP_HZ[c->v[RP_FREQ]]*(1ULL<<32)/FS;
 int64_t sn=sine_i(ph),cs=sine_i(ph+(1u<<30));
 int64_t a=RP_A[c->v[RP_GAIN]+12],alpha=(int32_t)(sn*100*16)/RP_Q100[c->v[RP_Q]];
 /* sin is Q15; alpha Q20. Q20 intermediate coefficients. */
 int64_t aa=alpha*a/(1<<20),ad=rp_div(alpha*(1<<20),(uint32_t)a),den=(1<<20)+ad;
 int64_t v[5]={(1<<20)+aa,-cs*64,(1<<20)-aa,-cs*64,(1<<20)-ad};
 for(unsigned j=0;j<5;j++)s->b[j]=rp_div(v[j]*(1LL<<28),(uint32_t)den);
 memset(s->h,0,sizeof s->h);s->applied=*c;
}
static int32_t rp_eq(rp_state_t *s,unsigned side,int32_t x){
 rp_hist_t *h=&s->h[side];x*=256;int64_t y=(int64_t)s->b[0]*x+(int64_t)s->b[1]*h->x1+(int64_t)s->b[2]*h->x2-(int64_t)s->b[3]*h->y1-(int64_t)s->b[4]*h->y2;
 int32_t o=(int32_t)clamp((int32_t)(y/(1LL<<28)),-268435456,268435456);
 h->x2=h->x1;h->x1=x;h->y2=h->y1;h->y1=o;return o/256;
}
/* side=1: L/R buffers, side=2: sum/difference (voice spread), side=0 mono.
 * A linked detector applies the same gain to both channels, preserving imaging. */
static void rp_process(unsigned unit,int32_t *l,int32_t *r,unsigned n,unsigned side){
 rp_state_t *s=&rp_state[unit];reggae_proc_config_t c=rp_config[unit];
 int changed=memcmp(c.v,s->applied.v,3*sizeof(int16_t))!=0;
 if(changed&&!s->wet){rp_coeff(s,&c);changed=0;}
 int target=(!changed&&c.v[RP_GAIN])?32768:0;
 unsigned ratio=c.v[RP_RATIO];
 if(!target&&!s->wet&&!ratio){s->env=0;s->gain=32768;s->reduction=0;return;}
 int atk=c.v[RP_ATTACK]?c.v[RP_ATTACK]:10,rel=c.v[RP_RELEASE]?c.v[RP_RELEASE]:150;
 int ak=32768/(44*atk),rk=32768/(44*rel);if(!rk)rk=1;
 for(unsigned i=0;i<n;i++){
  int32_t x=l[i],y=side?r[i]:x;
  if(side==2){y=(x-y)/2;x=l[i]-y;}
  x=clamp(x,-1048576,1048576);y=clamp(y,-1048576,1048576);
  s->wet+=clamp(target-s->wet,-256,256);
  if(s->wet){int32_t a=rp_eq(s,0,x),b=side?rp_eq(s,1,y):a;
   x+=(int32_t)((int64_t)(a-x)*s->wet/32768);y+=(int32_t)((int64_t)(b-y)*s->wet/32768);}
  if(ratio){
   int32_t pk=x<0?-x:x,pr=y<0?-y:y;if(pr>pk)pk=pr;
   int32_t diff=pk-s->env;s->env+=(int32_t)((int64_t)diff*(diff>0?ak:rk)/32768);
   if(!(s->phase++&15)){
    int32_t threshold=15*4096-c.v[RP_THRESH]*680;
    int32_t over=rp_log2(s->env?s->env:1)-threshold;
    unsigned gr=over>0?over*(RP_RATIO_VALUE[ratio]-1)/RP_RATIO_VALUE[ratio]:0;
    s->reduction=(uint16_t)(gr*100/680); /* hundredths dB */
    s->goal=rp_expneg(gr);
   }
   s->gain+=(s->goal-s->gain)/64;
   int32_t gain=(int64_t)s->gain*RP_MAKE[c.v[RP_MAKEUP]]/32768;
   x=(int32_t)((int64_t)x*gain/32768);y=(int32_t)((int64_t)y*gain/32768);
  }else{s->gain=32768;s->reduction=0;}
  /* Preserve headroom for the existing Q12 level/pan multiplies (max level 9713).
   * Spread sum/difference stays <=200000, well below signed multiply overflow. */
  x=clamp(x,-100000,100000);y=clamp(y,-100000,100000);
  if(side==2){l[i]=x+y;r[i]=x-y;}else{l[i]=x;if(side)r[i]=y;}
 }
}
