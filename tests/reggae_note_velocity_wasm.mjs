// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{}),e=instance.exports;
e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
const buttons=m=>{e.web_buttons(m);advance(80);};const keys=m=>{e.web_keys(m);advance(80);};
const tap=k=>{buttons(1<<k);buttons(0);advance(250);};
const turn=(k,n)=>{e.web_enc(k,n);advance(150);};
const energy=()=>{let sum=0;for(let b=0;b<1600;b++){e.web_render(128);for(const v of new Float32Array(e.memory.buffer,e.web_out_l(),128)){assert(Number.isFinite(v)&&Math.abs(v)<1);sum+=v*v;}}return sum;};
const play=()=>{tap(10);const v=energy();tap(10);advance(1500);return v;};
advance(1000);tap(9);buttons(1<<9);advance(700);turn(3,-100);buttons(0);advance(300);buttons(1<<4);keys((1<<0)|(1<<4)|(1<<7));keys(0);buttons(0);advance(1500);
const normal=play();assert(normal>0.1);
tap(9);tap(9);tap(9); // DETAIL, AUTOMATION, VELOCITY
turn(3,-100);turn(4,-100);turn(5,-127); // step 1, first note, velocity 1
const oneSoft=play();console.log({normal,oneSoft});assert(oneSoft>0.01&&oneSoft<normal*0.95,'one edited note softens the chord; other notes still sound');
turn(4,1);turn(5,-127);turn(4,1);turn(5,-127);
const allSoft=play();assert(allSoft<normal*0.1,'each note can be softened separately');
turn(4,-100);turn(6,-1);turn(4,1);turn(6,-1);turn(4,1);turn(6,-1);
const inherited=play();assert(inherited>allSoft*5&&inherited>normal*0.7,'inherit restores shared step velocities');
console.log('PASS: actual WASM SEQ/K1–K4 note selection, independent audible velocity changes and inheritance', {normal,oneSoft,allSoft,inherited});
