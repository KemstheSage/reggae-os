// SPDX-License-Identifier: GPL-3.0-only
// Test physical keyboard gestures and resulting pattern audio in the shipped WASM.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{});
const e=instance.exports;e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
const buttons=m=>{e.web_buttons(m);advance(80);};
const keys=m=>{e.web_keys(m);advance(80);};
const tap=k=>{buttons(1<<k);buttons(0);};
// Observe more than two complete 16-step loops; the first short note can end during tap().
const energy=()=>{let sum=0;for(let n=0;n<1600;n++){e.web_render(128);for(const v of new Float32Array(e.memory.buffer,e.web_out_l(),128)){assert(Number.isFinite(v));sum+=v*v;}}return sum;};
advance(1000);tap(9);
buttons(1<<4);keys((1<<0)|(1<<4)|(1<<7));keys(0);buttons(0);advance(1000);
tap(10);assert(energy()>0.1,'Added chord must play in pattern');tap(10);advance(1000);
buttons((1<<4)|(1<<11));buttons(0);advance(1000);
tap(10);assert(energy()<0.001,'Cleared pattern must be silent');tap(10);advance(1000);
buttons(1<<11);keys(1<<4);buttons(0);keys(0);advance(1000);
tap(10);assert(energy()>0.1,'Replacement note must play after modifier-first release');tap(10);
console.log('PASS: WASM EDIT-add chord, combined clear/silence, REC replacement and released keys');
