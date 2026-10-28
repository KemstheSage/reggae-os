/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
int main(void){
 web_nor_erase();web_boot();advance(1000);TSEL->p[P_VOICE]=V_POLY;
 const int expected[][4]={{0,2,7,-1},{0,4,7,9},{0,3,7,9},{0,4,7,14},{0,3,7,14},
 {0,4,10,14},{0,4,11,14},{0,3,10,14},{0,5,7,10},{0,3,6,-1},{0,3,6,9},{0,3,6,10},{0,4,8,-1},{0,3,7,11},{0,4,9,14}};
 const char *names[]={"Csus2","C6","Cm6","Cadd9","Cmadd9","C9","Cmaj9","Cm9","C7sus4","Cdim","Cdim7","Cm7b5","Caug","CmM7","C6/9"};
 assert(NELEM(N_CHRD)==CH_COUNT && TP[P_CHRD].max==CH_COUNT-1);
 for(unsigned i=0;i<NELEM(expected);i++){
  TSEL->p[P_CHRD]=CH_SUS2+i;TSEL->p[P_VOIC]=VC_CLOSE;
  uint8_t out[CHORD_MAX];int32_t root;uint16_t mask;char name[12];
  unsigned n=chord_make(TSEL,60,out,&root,&mask),want=expected[i][3]<0?3:4;assert(n==want);
  for(unsigned j=0;j<n;j++)assert(out[j]==60+expected[i][j]);
  chord_name(name,root,mask);assert(!strcmp(name,names[i]));
  for(unsigned v=0;v<=VC_BASS;v++)for(unsigned r=0;r<128;r++){
   TSEL->p[P_VOIC]=v;n=chord_make(TSEL,r,out,&root,&mask);assert(n>=1 && n<=4);
   for(unsigned j=0;j<n;j++){assert(out[j]<=127);if(j)assert(out[j]>out[j-1]);}
  }
  TSEL->p[P_VOIC]=VC_CLOSE;web_midi(0x90,60,100);advance(100);
  double energy=0;for(unsigned j=0;j<128;j++)energy+=out_l[j]*out_l[j]+out_r[j]*out_r[j];assert(energy>1e-6);
  TSEL->p[P_CHRD]=CH_OFF;web_midi(0x80,60,0);advance(1500);
  for(unsigned v=0;v<NVOICE;v++)assert(!TSEL->v[v].gate);
 }
 TSEL->p[P_CHRD]=CH_DOM9;TSEL->p[P_VOIC]=VC_BASS;
 uint8_t notes[4];int32_t root;uint16_t mask;assert(chord_make(TSEL,60,notes,&root,&mask)==4);
 assert(notes[0]==48 && notes[1]==64 && notes[2]==70 && notes[3]==74);
 assert(project_save(0)==0);TSEL->p[P_CHRD]=CH_OFF;TSEL->p[P_VOIC]=VC_CLOSE;project_load(0);assert(TSEL->p[P_CHRD]==CH_DOM9 && TSEL->p[P_VOIC]==VC_BASS);
 puts("PASS: 15 new shapes/names, 9600 range/voicing cases, audible MIDI chords, release after shape change, extended bass voicing and save");
}
