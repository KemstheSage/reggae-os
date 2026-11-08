/* SPDX-License-Identifier: GPL-3.0-only */
/* Bounded per-note timers. Ordinary patterns keep the legacy sequencer path.
 * Generations prevent an old timer releasing a newer live/sequenced note.
 * Time resolution is the existing audio control block, not browser timers. */
#define NT_SLOTS 16
static struct nt_event {uint32_t gen,off,next,period,duration;uint8_t note,vel,left,active,gate,trim;} nt_events[NTRK][NT_SLOTS] __attribute__((section(".pool")));
static uint32_t nt_clock[NTRK];
static void nt_off(track_t *t,struct nt_event *e){
 if(e->gen!=nt_generation[t-trk][e->note])return;
 if(e->trim && t->engine==ENGI_DRUM)for(unsigned i=0;i<NVOICE;i++)if(t->v[i].note==e->note&&t->v[i].gate){
  drum_lane_t *l=drum_lane_of(t,&t->v[i]);if(l)dv_choke(&l->v);
 }
 trk_note_off(t,e->note);e->gate=0;
}
static void nt_clear(track_t *t){for(unsigned i=0;i<NT_SLOTS;i++){struct nt_event *e=&nt_events[t-trk][i];if(e->active&&e->gate)nt_off(t,e);e->active=0;}}
static void nt_add(track_t *t,unsigned note,unsigned vel,unsigned duration,unsigned period,unsigned hits,unsigned trim){
 unsigned k=t-trk,slot=0;uint32_t now=nt_clock[k];
 for(unsigned i=0;i<NT_SLOTS;i++)if(nt_events[k][i].active&&nt_events[k][i].note==note){nt_off(t,&nt_events[k][i]);nt_events[k][i].active=0;}
 for(slot=0;slot<NT_SLOTS;slot++)if(!nt_events[k][slot].active)break;
 if(slot==NT_SLOTS){slot=0;for(unsigned i=1;i<NT_SLOTS;i++)if((int32_t)(nt_events[k][i].off-nt_events[k][slot].off)<0)slot=i;nt_off(t,&nt_events[k][slot]);}
 trk_note_on(t,note,vel);
 nt_events[k][slot]=(struct nt_event){nt_generation[k][note],now+duration,now+period,period,duration,note,vel,hits-1,1,1,trim};
}
static void nt_tick(track_t *t,unsigned n){
 unsigned k=t-trk;uint32_t now=(nt_clock[k]+=n);
 for(unsigned i=0;i<NT_SLOTS;i++){
  struct nt_event *e=&nt_events[k][i];if(!e->active)continue;
  if(e->gen!=nt_generation[k][e->note]){e->active=0;continue;}
  if(e->gate&&(int32_t)(now-e->off)>=0)nt_off(t,e);
  if(e->left&&(int32_t)(now-e->next)>=0){
   if(e->gate)nt_off(t,e);trk_note_on(t,e->note,e->vel);e->gen=nt_generation[k][e->note];e->gate=1;
   e->off=e->next+e->duration;e->next+=e->period;e->left--;
  }
  if(!e->gate&&!e->left)e->active=0;
 }
}
