// SPDX-License-Identifier: GPL-3.0-only
// Physical panel input and actual firmware audio; no browser sequencer.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{});
const e=instance.exports;e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let n=0;n<ms*44.1;n+=128)e.web_render(128);};
const tap=k=>{e.web_buttons(1<<k);advance(60);e.web_buttons(0);advance(100);};
const key=k=>{e.web_keys(1<<k);advance(100);e.web_keys(0);advance(100);};
const energy=()=>{let sum=0;for(let b=0;b<1600;b++){e.web_render(128);for(const v of new Float32Array(e.memory.buffer,e.web_out_l(),128)){assert(Number.isFinite(v));sum+=v*v;}}return sum;};
advance(1000);tap(11);assert(energy()<0.001,'Armed empty loop waits silently');
key(7);tap(11);assert(energy()>0.1,'First note is captured and loops after REC finishes');
e.web_enc(1,1);advance(150);tap(11);key(7);tap(11);assert(energy()>0.1);
for(let i=0;i<2;i++){e.web_enc(1,1);advance(150);}tap(11);key(0);tap(11);assert(energy()>0.1);
tap(10);advance(3000);assert(energy()<0.001,'Stopped layers release to silence');
console.log('PASS: WASM note-start, organ/bass/drum layering, audible loops and clean stop');
