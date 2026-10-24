/* SPDX-License-Identifier: GPL-3.0-only */
#define REGGAE_NO_MAIN 1
#include "reggae_test.c"
static void buttons(unsigned m){web_buttons(m);advance(80);}
static void keys(unsigned m){web_keys(m);advance(80);}
static void tap(unsigned k){buttons(1u<<k);buttons(0);}
int main(void) {
    web_nor_erase();web_boot();advance(1000);tap(9);
    assert(reggae_step_gesture_page());cursor_set(2);
    step_t *s=&TSEL->step[2];step_clear(s);s->time=ST_NOTE;s->n=1;s->note[0]=40;
    unsigned engine=TSEL->eng_req,rec=song.rec;
    uint8_t a=kb_map(TSEL,0),b=kb_map(TSEL,4),c=kb_map(TSEL,7);
    buttons(1u<<4);advance(600);assert(!ui.layer);
    keys(1);keys(0);keys(1u<<4);keys(0);keys(1);keys(0);buttons(0);
    assert(s->n==3&&s->note[0]==40&&s->note[1]==a&&s->note[2]==b);
    assert(ui.cursor==2&&TSEL->eng_req==engine&&song.rec==rec);
    tap(4);assert(s->n==3); /* EDIT alone never deletes */
    buttons(1u<<11);advance(500);assert(s->n==3); /* no key: no overwrite */
    keys((1u<<4)|(1u<<7));assert(s->n==2&&s->note[0]==b&&s->note[1]==c);
    buttons(0);advance(100);assert(ui.cursor==2);keys(0);advance(100);
    assert(ui.cursor==2&&song.rec==rec&&!ui.layer);
    /* Modifier and keys on the same scan, then modifier released first. */
    web_buttons(1u<<4);web_keys(1);advance(100);buttons(0);keys(0);
    assert(s->n==3&&s->note[2]==a&&ui.cursor==2);
    buttons((1u<<4)|(1u<<11));assert(!s->n&&s->time==ST_REST);buttons(0);
    assert(ui.cursor==2&&song.rec==rec);
    buttons(1u<<7);advance(800);buttons(0);assert(s->n==3); /* existing SAVE-hold undo */
    buttons((1u<<4)|(1u<<11));buttons(0);assert(!s->n);
    /* Tap REC retains normal arming. An add gesture must not advance its cursor. */
    tap(11);assert(song.rec==(rec^(1u<<song.sel)));
    buttons(1u<<4);keys(1);keys(0);buttons(0);advance(150);
    assert(s->n==1&&ui.cursor==2&&song.rec==(rec^(1u<<song.sel)));
    tap(11);assert(song.rec==rec);
    buttons(1u<<11);advance(700);buttons(0);assert(song.rec==rec&&s->n==1);
    /* Keys released: no stuck live voices. Sound/engine never changed. */
    advance(2000);for(unsigned v=0;v<NVOICE;v++)assert(!TSEL->v[v].active);
    assert(TSEL->eng_req==engine);
    picture("build/reggae/step-gestures.ppm");
    puts("PASS: EDIT adds/deduplicates, REC replaces chords, combined clear, fixed cursor, tap REC, no stuck notes");
}
